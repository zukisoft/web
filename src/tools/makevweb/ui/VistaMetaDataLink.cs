//---------------------------------------------------------------------------
// VistaMetaDataLink.cs
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
	public partial class VistaMetaDataLink : LinkLabel
	{
		//-------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------

		private const string FONTNAME_VISTA = "Segoe UI";
		private const string FONTNAME_XP = "Verdana";

		/// <summary>
		/// Default Constructor
		/// </summary>
		public VistaMetaDataLink()
		{
			InitializeComponent();			// Perform designer initializations

			// Set the properties we don't want to allow the designer/user to change

			base.LinkBehavior = LinkBehavior.HoverUnderline;
			base.UseCompatibleTextRendering = true;
			base.Font = new Font(s_isVista ? FONTNAME_VISTA : FONTNAME_XP, 10.0F, FontStyle.Regular);

			SetThemeColors();				// Apply initial control colors
		}

		//-------------------------------------------------------------------
		// Control Overrides
		//-------------------------------------------------------------------

		/// <summary>
		/// Changes the colors to something themed on Windows XP
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

		/// <summary>
		/// Replaces the standard LinkLabel.ActiveLinkColor property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new Color ActiveLinkColor
		{
			get { return base.ActiveLinkColor; }
			set { }
		}

		/// <summary>
		/// Replaces the standard LinkLabel.LinkBehavior property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new LinkBehavior LinkBehavior
		{
			get { return base.LinkBehavior; }
			set { }
		}

		/// <summary>
		/// Replaces the standard LinkLabel.LinkColor property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new Color LinkColor
		{
			get { return base.LinkColor; }
			set { }
		}

		/// <summary>
		/// Replaces the standard LinkLabel.VisitedLinkColor property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new Color VisitedLinkColor
		{
			get { return base.VisitedLinkColor; }
			set { }
		}
		/// <summary>
		/// Replaces the standard LinkLabel.VisitedLinkColor property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new bool UseCompatibleTextRendering
		{
			get { return base.UseCompatibleTextRendering; }
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
			// Under Vista or a non-themed system, use fixed colors for everything

			if (s_isVista || !VisualStyleRenderer.IsSupported)
			{
				base.ActiveLinkColor = Color.White;
				base.LinkColor = Color.White;
				base.VisitedLinkColor = Color.White;
				return;
			}

			// XP with themes enabled.  Use the GroupBox text foreground
			// color for all of the various LinkLabel states

			VisualStyleRenderer renderer = new VisualStyleRenderer(VisualStyleElement.Button.GroupBox.Normal);
			Color textColor = renderer.GetColor(ColorProperty.TextColor);

			base.ActiveLinkColor = textColor;
			base.LinkColor = textColor;
			base.VisitedLinkColor = textColor;
		}

		//-------------------------------------------------------------------
		// Member Variables
		//-------------------------------------------------------------------

		private static bool s_isVista = (Environment.OSVersion.Version.Major >= 6);
	}
}
