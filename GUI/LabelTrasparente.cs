using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace OCR
{
    public partial class LabelTrasparente : UserControl
    {
        private bool bPaintOnce = false;


        public LabelTrasparente()
        {
            InitializeComponent();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
        }
        
        protected override void OnPaint(PaintEventArgs e)
        {
            if (!bPaintOnce)
            {
                bPaintOnce = true;

                this.Visible = false;
                this.Parent.Invalidate(this.Bounds);
                this.Parent.Update();
                this.Visible = true;
                return;
            }
            else
            {
                bPaintOnce = false;

                Graphics g = e.Graphics;
                Font font = this.Font;
                SolidBrush brush = new SolidBrush(this.ForeColor);
                g.DrawString(this.Text, font, brush, 1, 1);
                g.Dispose();
            }

        }
    }
}
