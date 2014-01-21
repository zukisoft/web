//---------------------------------------------------------------------------
// VistaMainInstruction.cs
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
using System.Windows.Forms.VisualStyles;

namespace zuki.ui
{
	/// <summary>
	/// VistaMainInstruction is a specialization of the Label control
	/// to implement a "MainInstruction" theme element
	/// </summary>
	public partial class VistaMainInstruction : Label
	{
		//-------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------

		private const string FONTNAME_VISTA = "Segoe UI";
		private const string FONTNAME_XP = "Franklin Gothic Medium";

		/// <summary>
		/// Default Constructor
		/// </summary>
		public VistaMainInstruction()
		{
			InitializeComponent();			// Perform designer initializations

			// Set the properties we don't want to allow the designer/user to change

			base.Font = new Font(s_isVista ? FONTNAME_VISTA : FONTNAME_XP, 12.0F, FontStyle.Regular);

			SetThemeColors();				// Apply any thematic colors
		}

		//-------------------------------------------------------------------
		// Control Overrides
		//-------------------------------------------------------------------

		/// <summary>
		/// Changes the foreground color to something themed on Windows XP
		/// </summary>
		/// <param name="args">Standard event arguments</param>
		protected override void OnSystemColorsChanged(EventArgs args)
		{
			SetThemeColors();						// Change the colors as necessary
			base.OnSystemColorsChanged(args);		// Invoke the base class
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

		//-------------------------------------------------------------------
		// Private Member Functions
		//-------------------------------------------------------------------

		/// <summary>
		/// Changes any colors on this control as necessary for theme support
		/// </summary>
		private void SetThemeColors()
		{
			// Under Vista or a non-themed system, use a fixed foreground color

			if (s_isVista || !VisualStyleRenderer.IsSupported)
			{
				base.ForeColor = Color.FromArgb(0x00, 0x33, 0x99);
				return;
			}

			// XP with themes enabled.  Use the GroupBox text foreground color

			VisualStyleRenderer renderer = new VisualStyleRenderer(VisualStyleElement.Button.GroupBox.Normal);
			base.ForeColor = renderer.GetColor(ColorProperty.TextColor);
		}

		//-------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------

		private static bool s_isVista = (Environment.OSVersion.Version.Major >= 6);
	}
}
