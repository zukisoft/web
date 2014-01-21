namespace zuki.web.tools.makevweb
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.m_leftPanel = new System.Windows.Forms.Panel();
			this.m_version = new zuki.ui.VistaMetaDataLabel();
			this.m_logo = new System.Windows.Forms.PictureBox();
			this.m_buttonPanel = new System.Windows.Forms.Panel();
			this.m_generate = new zuki.ui.VistaCommandLink();
			this.m_workspacePanel = new System.Windows.Forms.Panel();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.m_excludeCommon = new System.Windows.Forms.CheckBox();
			this.m_optionsSeparator = new zuki.ui.VistaStaticSeparator();
			this.m_destBrowse = new zuki.ui.VistaLink();
			this.m_destinationFile = new System.Windows.Forms.TextBox();
			this.m_sourceBrowse = new zuki.ui.VistaLink();
			this.m_sourceFolder = new System.Windows.Forms.TextBox();
			this.m_destSeparator = new zuki.ui.VistaStaticSeparator();
			this.m_sourceSeparator = new zuki.ui.VistaStaticSeparator();
			this.m_heading = new zuki.ui.VistaMainInstruction();
			this.panel1 = new System.Windows.Forms.Panel();
			this.m_progressMessage = new System.Windows.Forms.Label();
			this.m_progress = new System.Windows.Forms.ProgressBar();
			this.m_sourceFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.m_destFileBrowser = new System.Windows.Forms.SaveFileDialog();
			this.m_leftPanel.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.m_logo)).BeginInit();
			this.m_buttonPanel.SuspendLayout();
			this.m_workspacePanel.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// m_leftPanel
			// 
			this.m_leftPanel.BackColor = System.Drawing.Color.LightSteelBlue;
			this.m_leftPanel.Controls.Add(this.m_version);
			this.m_leftPanel.Controls.Add(this.m_logo);
			this.m_leftPanel.Dock = System.Windows.Forms.DockStyle.Left;
			this.m_leftPanel.Location = new System.Drawing.Point(0, 0);
			this.m_leftPanel.Name = "m_leftPanel";
			this.m_leftPanel.Padding = new System.Windows.Forms.Padding(0, 0, 0, 4);
			this.m_leftPanel.Size = new System.Drawing.Size(180, 485);
			this.m_leftPanel.TabIndex = 0;
			// 
			// m_version
			// 
			this.m_version.BackColor = System.Drawing.Color.Transparent;
			this.m_version.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.m_version.Location = new System.Drawing.Point(0, 459);
			this.m_version.Name = "m_version";
			this.m_version.Size = new System.Drawing.Size(180, 22);
			this.m_version.TabIndex = 1;
			this.m_version.Text = "{version}";
			this.m_version.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// m_logo
			// 
			this.m_logo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_logo.BackColor = System.Drawing.Color.Transparent;
			this.m_logo.Image = ((System.Drawing.Image)(resources.GetObject("m_logo.Image")));
			this.m_logo.Location = new System.Drawing.Point(8, 8);
			this.m_logo.Name = "m_logo";
			this.m_logo.Size = new System.Drawing.Size(164, 164);
			this.m_logo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
			this.m_logo.TabIndex = 0;
			this.m_logo.TabStop = false;
			// 
			// m_buttonPanel
			// 
			this.m_buttonPanel.Controls.Add(this.m_generate);
			this.m_buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.m_buttonPanel.Location = new System.Drawing.Point(180, 410);
			this.m_buttonPanel.Name = "m_buttonPanel";
			this.m_buttonPanel.Size = new System.Drawing.Size(546, 75);
			this.m_buttonPanel.TabIndex = 2;
			// 
			// m_generate
			// 
			this.m_generate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.m_generate.Enabled = false;
			this.m_generate.Location = new System.Drawing.Point(410, 16);
			this.m_generate.Name = "m_generate";
			this.m_generate.Note = "";
			this.m_generate.Size = new System.Drawing.Size(116, 42);
			this.m_generate.TabIndex = 21;
			this.m_generate.Text = "Generate";
			this.m_generate.UseVisualStyleBackColor = true;
			this.m_generate.Click += new System.EventHandler(this.OnGenerate);
			// 
			// m_workspacePanel
			// 
			this.m_workspacePanel.Controls.Add(this.checkBox1);
			this.m_workspacePanel.Controls.Add(this.m_excludeCommon);
			this.m_workspacePanel.Controls.Add(this.m_optionsSeparator);
			this.m_workspacePanel.Controls.Add(this.m_destBrowse);
			this.m_workspacePanel.Controls.Add(this.m_destinationFile);
			this.m_workspacePanel.Controls.Add(this.m_sourceBrowse);
			this.m_workspacePanel.Controls.Add(this.m_sourceFolder);
			this.m_workspacePanel.Controls.Add(this.m_destSeparator);
			this.m_workspacePanel.Controls.Add(this.m_sourceSeparator);
			this.m_workspacePanel.Controls.Add(this.m_heading);
			this.m_workspacePanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.m_workspacePanel.Location = new System.Drawing.Point(180, 0);
			this.m_workspacePanel.Name = "m_workspacePanel";
			this.m_workspacePanel.Size = new System.Drawing.Size(546, 342);
			this.m_workspacePanel.TabIndex = 3;
			// 
			// checkBox1
			// 
			this.checkBox1.Enabled = false;
			this.checkBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkBox1.Location = new System.Drawing.Point(32, 300);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(424, 17);
			this.checkBox1.TabIndex = 20;
			this.checkBox1.Text = "Enable compression";
			this.checkBox1.UseVisualStyleBackColor = true;
			this.checkBox1.Visible = false;
			// 
			// m_excludeCommon
			// 
			this.m_excludeCommon.Checked = true;
			this.m_excludeCommon.CheckState = System.Windows.Forms.CheckState.Checked;
			this.m_excludeCommon.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.m_excludeCommon.Location = new System.Drawing.Point(32, 278);
			this.m_excludeCommon.Name = "m_excludeCommon";
			this.m_excludeCommon.Size = new System.Drawing.Size(416, 17);
			this.m_excludeCommon.TabIndex = 19;
			this.m_excludeCommon.Text = "Exclude non-virtualizable ASP.NET files and folders";
			this.m_excludeCommon.UseVisualStyleBackColor = true;
			// 
			// m_optionsSeparator
			// 
			this.m_optionsSeparator.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_optionsSeparator.Location = new System.Drawing.Point(20, 248);
			this.m_optionsSeparator.Name = "m_optionsSeparator";
			this.m_optionsSeparator.Size = new System.Drawing.Size(506, 15);
			this.m_optionsSeparator.TabIndex = 18;
			this.m_optionsSeparator.Text = "Options";
			// 
			// m_destBrowse
			// 
			this.m_destBrowse.Location = new System.Drawing.Point(32, 208);
			this.m_destBrowse.Name = "m_destBrowse";
			this.m_destBrowse.Size = new System.Drawing.Size(144, 20);
			this.m_destBrowse.TabIndex = 17;
			this.m_destBrowse.TabStop = true;
			this.m_destBrowse.Text = "Browse locations";
			this.m_destBrowse.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnBrowseDestination);
			// 
			// m_destinationFile
			// 
			this.m_destinationFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_destinationFile.Location = new System.Drawing.Point(32, 182);
			this.m_destinationFile.Name = "m_destinationFile";
			this.m_destinationFile.Size = new System.Drawing.Size(494, 23);
			this.m_destinationFile.TabIndex = 16;
			this.m_destinationFile.TextChanged += new System.EventHandler(this.OnDestinationChanged);
			// 
			// m_sourceBrowse
			// 
			this.m_sourceBrowse.Location = new System.Drawing.Point(32, 112);
			this.m_sourceBrowse.Name = "m_sourceBrowse";
			this.m_sourceBrowse.Size = new System.Drawing.Size(144, 20);
			this.m_sourceBrowse.TabIndex = 15;
			this.m_sourceBrowse.TabStop = true;
			this.m_sourceBrowse.Text = "Browse folders";
			this.m_sourceBrowse.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnBrowseSource);
			// 
			// m_sourceFolder
			// 
			this.m_sourceFolder.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_sourceFolder.Location = new System.Drawing.Point(32, 86);
			this.m_sourceFolder.Name = "m_sourceFolder";
			this.m_sourceFolder.Size = new System.Drawing.Size(494, 23);
			this.m_sourceFolder.TabIndex = 13;
			this.m_sourceFolder.TextChanged += new System.EventHandler(this.OnSourceChanged);
			// 
			// m_destSeparator
			// 
			this.m_destSeparator.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_destSeparator.Location = new System.Drawing.Point(20, 152);
			this.m_destSeparator.Name = "m_destSeparator";
			this.m_destSeparator.Size = new System.Drawing.Size(506, 15);
			this.m_destSeparator.TabIndex = 10;
			this.m_destSeparator.Text = "Destination File";
			// 
			// m_sourceSeparator
			// 
			this.m_sourceSeparator.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_sourceSeparator.Location = new System.Drawing.Point(20, 56);
			this.m_sourceSeparator.Name = "m_sourceSeparator";
			this.m_sourceSeparator.Size = new System.Drawing.Size(506, 15);
			this.m_sourceSeparator.TabIndex = 9;
			this.m_sourceSeparator.Text = "Source Folder";
			// 
			// m_heading
			// 
			this.m_heading.AutoSize = true;
			this.m_heading.Location = new System.Drawing.Point(18, 16);
			this.m_heading.Name = "m_heading";
			this.m_heading.Size = new System.Drawing.Size(280, 21);
			this.m_heading.TabIndex = 7;
			this.m_heading.Text = "Generate a Zucchini Virtual File System";
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.m_progressMessage);
			this.panel1.Controls.Add(this.m_progress);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.panel1.Location = new System.Drawing.Point(180, 342);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(546, 68);
			this.panel1.TabIndex = 4;
			// 
			// m_progressMessage
			// 
			this.m_progressMessage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_progressMessage.Location = new System.Drawing.Point(20, 10);
			this.m_progressMessage.Name = "m_progressMessage";
			this.m_progressMessage.Size = new System.Drawing.Size(506, 17);
			this.m_progressMessage.TabIndex = 14;
			this.m_progressMessage.Text = "Processing Folder /webHelp/Images";
			this.m_progressMessage.Visible = false;
			// 
			// m_progress
			// 
			this.m_progress.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.m_progress.Location = new System.Drawing.Point(23, 34);
			this.m_progress.MarqueeAnimationSpeed = 75;
			this.m_progress.Name = "m_progress";
			this.m_progress.Size = new System.Drawing.Size(503, 19);
			this.m_progress.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
			this.m_progress.TabIndex = 13;
			this.m_progress.Visible = false;
			// 
			// m_sourceFolderBrowser
			// 
			this.m_sourceFolderBrowser.Description = "Select the root folder of the ASP.NET Application or ASP.NET Web Service to be co" +
				"nverted into a Virtual File System";
			this.m_sourceFolderBrowser.ShowNewFolderButton = false;
			// 
			// m_destFileBrowser
			// 
			this.m_destFileBrowser.DefaultExt = "vweb";
			this.m_destFileBrowser.Filter = "Zucchini Virtual File Systems (*.vweb)|*.vweb|All Files (*.*)|*.*";
			this.m_destFileBrowser.SupportMultiDottedExtensions = true;
			this.m_destFileBrowser.Title = "Select Virtual File System location";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
			this.BackColor = System.Drawing.SystemColors.Window;
			this.ClientSize = new System.Drawing.Size(716, 475);
			this.Controls.Add(this.m_workspacePanel);
			this.Controls.Add(this.panel1);
			this.Controls.Add(this.m_buttonPanel);
			this.Controls.Add(this.m_leftPanel);
			this.DoubleBuffered = true;
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Zucchini Virtual File System Generator";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.Load += new System.EventHandler(this.OnFormLoad);
			this.m_leftPanel.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.m_logo)).EndInit();
			this.m_buttonPanel.ResumeLayout(false);
			this.m_workspacePanel.ResumeLayout(false);
			this.m_workspacePanel.PerformLayout();
			this.panel1.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel m_leftPanel;
		private System.Windows.Forms.PictureBox m_logo;
		private zuki.ui.VistaMetaDataLabel m_version;
		private System.Windows.Forms.Panel m_buttonPanel;
		private System.Windows.Forms.Panel m_workspacePanel;
		private zuki.ui.VistaMainInstruction m_heading;
		private zuki.ui.VistaStaticSeparator m_sourceSeparator;
		private zuki.ui.VistaStaticSeparator m_destSeparator;
		private System.Windows.Forms.TextBox m_sourceFolder;
		private zuki.ui.VistaLink m_sourceBrowse;
		private zuki.ui.VistaLink m_destBrowse;
		private System.Windows.Forms.TextBox m_destinationFile;
		private zuki.ui.VistaStaticSeparator m_optionsSeparator;
		private System.Windows.Forms.CheckBox m_excludeCommon;
		private System.Windows.Forms.CheckBox checkBox1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label m_progressMessage;
		private System.Windows.Forms.ProgressBar m_progress;
		private System.Windows.Forms.FolderBrowserDialog m_sourceFolderBrowser;
		private System.Windows.Forms.SaveFileDialog m_destFileBrowser;
		private zuki.ui.VistaCommandLink m_generate;


	}
}

