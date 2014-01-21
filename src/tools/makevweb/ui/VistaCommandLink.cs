//---------------------------------------------------------------------------
// VistaCommandLink.cs
//
// ZukiSoft Vista UI Elements
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
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace zuki.ui
{
	/// <summary>
	/// VistaCommandLink is just a quick customization of Button to turn it
	/// into a Command Link under Windows Vista (and higher).  While this code
	/// will work under Windows XP, you'll end up with a very very large button --
	/// it would be better to not use it all on the legacy platforms
	/// </summary>
	public partial class VistaCommandLink : Button
	{
		/// <summary>
		/// Win32 API Declarations
		/// </summary>
		private class Win32
		{
			[DllImport("user32.dll", CharSet = CharSet.Unicode)]
			public static extern IntPtr SendMessage(HandleRef hWnd, UInt32 Msg, IntPtr wParam, string lParam);

			public const uint BCM_SETNOTE = 0x00001609;
			public const uint BS_COMMANDLINK = 0x0000000E;
		}

		/// <summary>
		/// Instances constructor
		/// </summary>
		public VistaCommandLink() 
		{ 
			InitializeComponent();
			base.FlatStyle = FlatStyle.System;
		}

		/// <summary>
		/// Overrides the CreateParams property to add BS_COMMANDLINK to the window
		/// style flags if running under Windows Vista or higher
		/// </summary>
		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams cp = base.CreateParams;
				if(s_isVista) cp.Style |= (int)Win32.BS_COMMANDLINK;
				return cp;
			}
		}

		/// <summary>
		/// Adds a new property to handle the NOTE text that appears on the button
		/// </summary>
		[Category("Appearance")]
		public string Note
		{
			get { return m_note; }
			set { m_note = value; SetNote(m_note); }
		}

		/// <summary>
		/// Replaces the standard Button.FlatStyle property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new FlatStyle FlatStyle
		{
			get { return base.FlatStyle; }
			set { }
		}

		//-------------------------------------------------------------------
		// Private Member Functions
		//-------------------------------------------------------------------

		/// <summary>
		/// Sends the BCM_SENDNOTE message to the button control
		/// </summary>
		/// <param name="NoteText">The new "note" text to be set</param>
		private void SetNote(string NoteText)
		{
			if (!s_isVista) return;
			Win32.SendMessage(new HandleRef(this, this.Handle), Win32.BCM_SETNOTE, IntPtr.Zero, NoteText);
		}

		//-------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------

		private string m_note = String.Empty;
		private static bool s_isVista = (Environment.OSVersion.Version.Major >= 6);
	}
}
