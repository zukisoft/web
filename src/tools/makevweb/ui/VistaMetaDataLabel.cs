//---------------------------------------------------------------------------
// VistaMetaDataLabel.cs
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
using System.Text;
using System.Windows.Forms;

namespace zuki.ui
{
	/// <summary>
	/// VistaMainInstruction is a specialization of the Label control
	/// to implement a "MetaDataLabel" theme element
	/// </summary>
	public partial class VistaMetaDataLabel : Label
	{
		//-------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------

		private const string FONTNAME_VISTA = "Segoe UI";
		private const string FONTNAME_XP = "Verdana";

		/// <summary>
		/// Default Constructor
		/// </summary>
		public VistaMetaDataLabel()
		{
			InitializeComponent();			// Perform designer initializations

			// Create the font and foreground color for this control, they are
			// constant and cannot be changed by the designer nor at runtime

			base.ForeColor = Color.White;
			base.Font = new Font((Environment.OSVersion.Version.Major >= 6) ?
				FONTNAME_VISTA : FONTNAME_XP, 10.0F, FontStyle.Regular);
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

		/// <summary>
		/// Adds the ability to change the font size, but not the font itself
		/// </summary>
		[Browsable(true), Category("Appearance")]
		public float FontSize
		{
			get { return base.Font.SizeInPoints; }
			set { base.Font = new Font(base.Font.FontFamily, value, base.Font.Style); }
		}

		/// <summary>
		/// Adds the ability to change the font style, but not the font itself
		/// </summary>
		[Browsable(true), Category("Appearance")]
		public FontStyle FontStyle
		{
			get { return base.Font.Style; }
			set { base.Font = new Font(base.Font, value); }
		}
	}
}
