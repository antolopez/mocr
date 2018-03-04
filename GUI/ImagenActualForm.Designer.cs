namespace OCR
{
    partial class ImagenActualForm
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.textoOriginalPictureBox = new System.Windows.Forms.PictureBox();
            this.textoOriginalPanel = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.textoOriginalPictureBox)).BeginInit();
            this.textoOriginalPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // textoOriginalPictureBox
            // 
            this.textoOriginalPictureBox.Location = new System.Drawing.Point(0, 0);
            this.textoOriginalPictureBox.Name = "textoOriginalPictureBox";
            this.textoOriginalPictureBox.Size = new System.Drawing.Size(495, 387);
            this.textoOriginalPictureBox.TabIndex = 0;
            this.textoOriginalPictureBox.TabStop = false;
            this.textoOriginalPictureBox.Paint += new System.Windows.Forms.PaintEventHandler(this.textoOriginalPictureBox_Paint);
            // 
            // textoOriginalPanel
            // 
            this.textoOriginalPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textoOriginalPanel.AutoScroll = true;
            this.textoOriginalPanel.Controls.Add(this.textoOriginalPictureBox);
            this.textoOriginalPanel.Location = new System.Drawing.Point(-1, 0);
            this.textoOriginalPanel.Name = "textoOriginalPanel";
            this.textoOriginalPanel.Size = new System.Drawing.Size(495, 387);
            this.textoOriginalPanel.TabIndex = 3;
            // 
            // ImagenActualForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(494, 387);
            this.Controls.Add(this.textoOriginalPanel);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "ImagenActualForm";
            this.ShowIcon = false;
            this.Text = "ImagenActual";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ImagenActualForm_FormClosed);
            this.Resize += new System.EventHandler(this.ImagenActualForm_Resize);
            ((System.ComponentModel.ISupportInitialize)(this.textoOriginalPictureBox)).EndInit();
            this.textoOriginalPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel textoOriginalPanel;
        public System.Windows.Forms.PictureBox textoOriginalPictureBox;

    }
}