//---------------------------------------------------------------------------
// VistaForm.cs
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
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace zuki.ui
{
	/// <summary>
	/// VistaForm isn't anything special, it just locks down the font for
	/// the entire form based on the operating system, like the other cheesy
	/// "Vista" controls I've thrown in here.
	/// </summary>
	public partial class VistaForm : Form
	{
		//-------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------

		private const string FONTNAME_VISTA = "Segoe UI";
		private const string FONTNAME_XP = "Tahoma";

		/// <summary>
		/// Default Constructor
		/// </summary>
		public VistaForm()
		{
			InitializeComponent();			// Perform designer initializations

			// Create the font and foreground color for this control, they are
			// constant and cannot be changed by the designer nor at runtime

			base.ForeColor = Color.Black;
			base.Font = new Font((Environment.OSVersion.Version.Major >= 6) ?
				FONTNAME_VISTA : FONTNAME_XP, 9.0F, FontStyle.Regular);
		}

		/// <summary>
		/// Replaces the standard Control.Font property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new Font Font
		{
			get { return base.Font; }
			set { }
		}

		/// <summary>
		/// Replaces the standard Control.ForeColor property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new Color ForeColor
		{
			get { return base.ForeColor; }
			set { }
		}
	}
}
