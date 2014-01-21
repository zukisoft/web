//---------------------------------------------------------------------------
// MainForm.cs
//
// Zucchini Virtual File System Generator
//
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or any other,
// from this software.
//
// Contributor(s):
//	Michael G. Brehm (original author)
//---------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Xml;
using zuki.data.structured;

namespace zuki.web.tools.makevweb
{
	public partial class MainForm : zuki.ui.VistaForm
	{
		public MainForm()
		{
			InitializeComponent();

			// Adjust the background color of the left panel to the theme,
			// but don't do it if it's white since it will absorb the version

			Color backColor = SystemColors.InactiveCaption;
			if (backColor != Color.White) m_leftPanel.BackColor = backColor;
		}

		//-------------------------------------------------------------------
		// Event Handlers
		//-------------------------------------------------------------------

		/// <summary>
		/// Invoked when the Destination Location browse link is clicked
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard LinkLabel event arguments</param>
		private void OnBrowseDestination(object sender, LinkLabelLinkClickedEventArgs args)
		{
			if (m_destFileBrowser.ShowDialog(this) != DialogResult.OK) return;
			m_destinationFile.Text = m_destFileBrowser.FileName;
		}

		/// <summary>
		/// Invoked when the Source Location browse link is clicked
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard LinkLabel event arguments</param>
		private void OnBrowseSource(object sender, LinkLabelLinkClickedEventArgs args)
		{
			if (m_sourceFolderBrowser.ShowDialog(this) != DialogResult.OK) return;
			m_sourceFolder.Text = m_sourceFolderBrowser.SelectedPath;
		}

		/// <summary>
		/// Invoked when the destination file location has been changed
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard event arguments</param>
		private void OnDestinationChanged(object sender, EventArgs args)
		{
			EnableDisableGenerate();		// Enable or disable generation
		}

		/// <summary>
		/// Invoked when the form is being closed
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard event arguments</param>
		private void OnFormClosing(object sender, FormClosingEventArgs args)
		{
			if (m_worker == null) return;
			m_worker.Abort();
			m_worker.Join();
		}

		/// <summary>
		/// Invoked when the form has been loaded
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard event arguments</param>
		private void OnFormLoad(object sender, EventArgs args)
		{
			m_version.Text = "Zucchini " + this.GetType().Assembly.GetName().Version.ToString(2);
		}

		/// <summary>
		/// Invoked when the GENERATE button has been clicked
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard event arguments</param>
		private void OnGenerate(object sender, EventArgs args)
		{
			// Disable the option controls and the generate button, and
			// enable the progress bar/message controls

			m_sourceFolder.Enabled = false;
			m_sourceBrowse.Enabled = false;
			m_destinationFile.Enabled = false;
			m_destBrowse.Enabled = false;
			m_excludeCommon.Enabled = false;
			m_generate.Enabled = false;
			Cursor = Cursors.WaitCursor;

			m_progress.Visible = true;
			m_progressMessage.Visible = true;
			this.Refresh();

			m_worker = new Thread(new ThreadStart(WorkerThread));
			m_worker.IsBackground = true;
			m_worker.Start();
		}

		/// <summary>
		/// Invoked when the source location has been changed by the user
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Standard event arguments</param>
		private void OnSourceChanged(object sender, EventArgs args)
		{
			EnableDisableGenerate();		// Enable or disable generation
		}

		//-------------------------------------------------------------------
		// Private Type Declarations
		//-------------------------------------------------------------------

		private delegate void WorkerExceptionHandler(Exception excption);
		private delegate void WorkerFinishedHandler();
		private delegate void WorkerProgressHandler(string message);

		//-------------------------------------------------------------------
		// Private Member Functions
		//-------------------------------------------------------------------

		/// <summary>
		/// Enables or disables the GENERATE button based on the validity
		/// of the user's source and destination options
		/// </summary>
		void EnableDisableGenerate()
		{
			bool enable = true;				// Assume we can enable generation

			// If the source folder name is not valid or the destination
			// file folder name is not valid, we can't generate the VWEB

			try
			{
				if (!Directory.Exists(m_sourceFolder.Text)) enable = false;
				if (!Directory.Exists(Path.GetDirectoryName(m_destinationFile.Text))) enable = false;
			}
			catch { enable = false; }

			m_generate.Enabled = enable;	// Enable or disable the button
		}

		/// <summary>
		/// Determines if the specified file is a site map XML file or not
		/// </summary>
		/// <param name="filename">The filename to be tested</param>
		/// <returns>True if the file looks like a site map XML file</returns>
		bool IsFileSiteMapXml(string filename)
		{
			try
			{
				XmlDocument doc = new XmlDocument();
				doc.Load(filename);
				return doc.DocumentElement.Name.Equals("sitemap", StringComparison.OrdinalIgnoreCase);
			}

			catch (Exception) { return false; }		// Assume not a site map
		}

		/// <summary>
		/// Special case version of ProcessStorage folder to deal with the root ASP.NET
		/// folder exclusions like web.config, \bin, \app_code, etc.
		/// </summary>
		/// <param name="path">The current physical path to load</param>
		/// <param name="container">The StorageContainer to be loaded</param>
		void ProcessRootStorageFolderSpecial(string path, StorageContainer container, WorkerProgressHandler progress)
		{
			byte[] buffer = new byte[8192];		// Local file buffer array
			int read;							// Bytes read from the file
			string nameOnly;					// File name only portion of the path

			Invoke(progress, new object[] { "Processing folder " + path });

			// Add all of the files into the StorageContainer by creating new
			// StorageObjects for each of them and copying the data 8KB at a time

			foreach (string filename in Directory.GetFiles(path))
			{
				nameOnly = Path.GetFileName(filename);		// Get the file name

				// SPECIAL CASE FILES
				if (nameOnly.StartsWith("global.asax", StringComparison.OrdinalIgnoreCase)) continue;
				if (nameOnly.Equals("web.config", StringComparison.OrdinalIgnoreCase)) continue;
				if ((Path.GetExtension(nameOnly) == ".xml") && (IsFileSiteMapXml(filename))) continue;

				StorageObject obj = container.Objects.Add(Path.GetFileName(nameOnly));
				
				using (FileStream reader = File.OpenRead(filename))
				{
					using (StorageObjectStream writer = obj.GetWriter())
					{
						do
						{
							read = reader.Read(buffer, 0, 8192);
							if (read > 0) writer.Write(buffer, 0, read);

						} while (read > 0);
					}
				}
			}

			// Now recursively create all of the subdirectories and process them

			foreach (string folder in Directory.GetDirectories(path))
			{
				nameOnly = Path.GetFileName(folder);		// Get the folder name

				// SPECIAL CASE FOLDERS
				if (nameOnly.Equals("bin", StringComparison.OrdinalIgnoreCase)) continue;
				if (nameOnly.Equals("app_code", StringComparison.OrdinalIgnoreCase)) continue;
				if (nameOnly.Equals("app_data", StringComparison.OrdinalIgnoreCase)) continue;
				if (nameOnly.Equals("app_globalresources", StringComparison.OrdinalIgnoreCase)) continue;
				if (nameOnly.Equals("app_localresources", StringComparison.OrdinalIgnoreCase)) continue;

				StorageContainer cont = container.Containers.Add(Path.GetFileName(nameOnly));
				ProcessStorageFolder(folder, cont, progress);
			}
		}

		/// <summary>
		/// Loads the contents of a physical folder into structured storage
		/// </summary>
		/// <param name="path">The current physical path to load</param>
		/// <param name="container">The StorageContainer to be loaded</param>
		void ProcessStorageFolder(string path, StorageContainer container, WorkerProgressHandler progress)
		{
			byte[] buffer = new byte[8192];		// Local file buffer array
			int read;							// Bytes read from the file

			Invoke(progress, new object[] { "Processing folder " + path });

			// Add all of the files into the StorageContainer by creating new
			// StorageObjects for each of them and copying the data 8KB at a time

			foreach (string filename in Directory.GetFiles(path))
			{
				StorageObject obj = container.Objects.Add(Path.GetFileName(filename));

				using (FileStream reader = File.OpenRead(filename))
				{
					using (StorageObjectStream writer = obj.GetWriter())
					{
						do
						{
							read = reader.Read(buffer, 0, 8192);
							if (read > 0) writer.Write(buffer, 0, read);

						} while (read > 0);
					}
				}
			}

			// Now recursively create all of the subdirectories and process them

			foreach (string folder in Directory.GetDirectories(path))
			{
				StorageContainer cont = container.Containers.Add(Path.GetFileName(folder));
				ProcessStorageFolder(folder, cont, progress);
			}
		}

		/// <summary>
		/// Invoked from the worker thread when an exception has occurred
		/// </summary>
		/// <param name="exception">The exception that occurred</param>
		void WorkerException(Exception exception)
		{
			MessageBox.Show(this, "The Virtual File System could not be generated\r\n\r\n" + 
				"[" + exception.Source + "]\r\n" + exception.Message, "Generation Error", 
				MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		/// <summary>
		/// Invoked from the worker thread when it finishes processing 
		/// </summary>
		void WorkerFinished()
		{
			// Re-enable all of the controls that were disabled during generation

			m_sourceFolder.Enabled = true;
			m_sourceBrowse.Enabled = true;
			m_destinationFile.Enabled = true;
			m_destBrowse.Enabled = true;
			m_excludeCommon.Enabled = true;
			m_generate.Enabled = true;
			Cursor = Cursors.Default;

			// NOTE: The progress message stays visible after file creation,
			// but we change the message to indicate what was done

			m_progress.Visible = false;
			this.Refresh();

			m_worker = null;				// Reset the reference to null
		}

		/// <summary>
		/// Updates the progress message from the worker thread
		/// </summary>
		/// <param name="message">The new progress message to be set</param>
		void WorkerProgress(string message)
		{
			m_progressMessage.Text = message;
		}

		/// <summary>
		/// Worker thread that actually performs the file generation
		/// </summary>
		void WorkerThread()
		{
			string destFile = m_destinationFile.Text;

			// Set up the necessary delegates we need to invoke methods on the main thread

			WorkerExceptionHandler onException = new WorkerExceptionHandler(WorkerException);
			WorkerFinishedHandler onFinished = new WorkerFinishedHandler(WorkerFinished);
			WorkerProgressHandler onProgress = new WorkerProgressHandler(WorkerProgress);

			try
			{
				// Attempt to create/replace the main structured storage file.  Notice the
				// completely meaningless pause for dramatic effect

				Invoke(onProgress, new object[] { "Creating the Virtual File System structured storage file" });
				Thread.Sleep(500);

				using (StructuredStorage storage = StructuredStorage.Open(destFile, StorageOpenMode.Create, StorageAccessMode.Exclusive))
				{
					// If the user did not elect to filter out non-virtualizable ASP.NET stuff,
					// we can just rip and roar through this without any additional work. Otherwise
					// use the special case handler for exclusing root items from the VFS

					if (!m_excludeCommon.Checked) ProcessStorageFolder(m_sourceFolder.Text, storage, onProgress);
					else ProcessRootStorageFolderSpecial(m_sourceFolder.Text, storage, onProgress);
				}

				Invoke(onProgress, new object[] { "The Virtual File System was generated successfully" });
				Invoke(onFinished);
			}

			catch (ThreadAbortException)
			{
				// If the thread is aborting, that means the form was closed while the
				// process was still active.  Just nuke the file and exit the thread

				if (File.Exists(destFile)) File.Delete(destFile);
			}

			catch (Exception exception)
			{
				if (File.Exists(destFile)) File.Delete(destFile);

				// Standard exceptions get pumped back into the main thread to be
				// reported. Also invoke the finish handler since I'm not really
				// spending a lot of time making this a top-notch application :)

				Invoke(onException, new object[] { exception });
				Invoke(onProgress, new object[] { "The Virtual File System could not be generated" });
				Invoke(onFinished);
			}
		}

		//-------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------

		private Thread m_worker;				// Storage creation worker thread
	}
}