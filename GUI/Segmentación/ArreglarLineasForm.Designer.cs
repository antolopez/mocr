namespace OCR.Segmentación
{
    partial class ArreglarLineasForm
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
            this.lineasSegmentadasPictureBox = new System.Windows.Forms.PictureBox();
            this.juntarButton = new System.Windows.Forms.Button();
            this.dividirButton = new System.Windows.Forms.Button();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.lineasSegmentadasPictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // lineasSegmentadasPictureBox
            // 
            this.lineasSegmentadasPictureBox.Location = new System.Drawing.Point(12, 12);
            this.lineasSegmentadasPictureBox.Name = "lineasSegmentadasPictureBox";
            this.lineasSegmentadasPictureBox.Size = new System.Drawing.Size(308, 460);
            this.lineasSegmentadasPictureBox.TabIndex = 0;
            this.lineasSegmentadasPictureBox.TabStop = false;
            this.lineasSegmentadasPictureBox.MouseClick += new System.Windows.Forms.MouseEventHandler(this.lineasSegmentadasPictureBox_MouseClick);
            // 
            // juntarButton
            // 
            this.juntarButton.Location = new System.Drawing.Point(355, 277);
            this.juntarButton.Name = "juntarButton";
            this.juntarButton.Size = new System.Drawing.Size(210, 74);
            this.juntarButton.TabIndex = 1;
            this.juntarButton.Text = "Fusionar";
            this.juntarButton.UseVisualStyleBackColor = true;
            this.juntarButton.Click += new System.EventHandler(this.juntarButton_Click);
            // 
            // dividirButton
            // 
            this.dividirButton.Location = new System.Drawing.Point(355, 138);
            this.dividirButton.Name = "dividirButton";
            this.dividirButton.Size = new System.Drawing.Size(210, 74);
            this.dividirButton.TabIndex = 1;
            this.dividirButton.Text = "Dividir";
            this.dividirButton.UseVisualStyleBackColor = true;
            this.dividirButton.Click += new System.EventHandler(this.dividirButton_Click);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(124, 498);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 3;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(318, 498);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 4;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // ArreglarLineasForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(606, 553);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.dividirButton);
            this.Controls.Add(this.juntarButton);
            this.Controls.Add(this.lineasSegmentadasPictureBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ArreglarLineasForm";
            this.Text = "Arreglar Líneas";
            ((System.ComponentModel.ISupportInitialize)(this.lineasSegmentadasPictureBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox lineasSegmentadasPictureBox;
        private System.Windows.Forms.Button juntarButton;
        private System.Windows.Forms.Button dividirButton;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
    }
}