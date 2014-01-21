//---------------------------------------------------------------------------
// VistaStaticSeparator.cs
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
	/// VistaStaticSepartor is a specialization of the Label control
	/// to implement a "StaticSeparator" theme element
	/// </summary>
	[ToolboxItem(true)]
	public partial class VistaStaticSeparator : Label
	{
		//-------------------------------------------------------------------
		// Constants
		//-------------------------------------------------------------------

		private const string FONTNAME_VISTA = "Segoe UI";
		private const string FONTNAME_XP = "Tahoma";

		/// <summary>
		/// Default Constructor
		/// </summary>
		public VistaStaticSeparator()
		{
			InitializeComponent();			// Perform designer initializations

			// Create the font and foreground color for this control, they are
			// constant and cannot be changed by the designer nor at runtime

			base.ForeColor = Color.Black;
			base.Font = new Font((Environment.OSVersion.Version.Major >= 6) ?
				FONTNAME_VISTA : FONTNAME_XP, 9.0F, FontStyle.Regular);

			// Force the remaining properties we're disallowing to the proper state
			base.AutoSize = false;
			base.TextAlign = ContentAlignment.MiddleLeft;
		}
		
		/// <summary>
		/// Overriden paint handler
		/// </summary>
		/// <param name="args"></param>
		protected override void OnPaint(PaintEventArgs args)
		{
			float		textLength;			// Length of the label text
			int			spaceToFill;		// Number of pixels to be filled

			// Determine how much space we need to fill up by measuring the string
			// and then using the difference between that and the width of the control

			textLength = args.Graphics.MeasureString(base.Text, base.Font, base.Width, StringFormat.GenericDefault).Width;
			spaceToFill = base.Width - (int)textLength;

			// The Vista static separtor appears to always use a 1px LightGray line that
			// is 1 pixel under the midline of the control

			using (Pen pen = new Pen(new SolidBrush(Color.LightGray), 1.0F))
			{
				float middle = this.Height / 2 + 2;
				args.Graphics.DrawLine(pen, textLength + 1, middle, this.Width, middle);
			}

			base.OnPaint(args);				// Invoke the base class version
		}

		/// <summary>
		/// Replaces the standard .AutoSize property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new bool AutoSize
		{
			get { return base.AutoSize; }
			set { }
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
		/// Replaces the standard .TextAlign property to prevent it from being set
		/// </summary>
		[Browsable(false), DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
		public new ContentAlignment TextAlign
		{
			get { return base.TextAlign; }
			set { }
		}
	}
}
