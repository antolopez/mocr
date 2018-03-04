namespace OCR.Ayuda
{
    partial class AcercaDeForm
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
            this.aceptarButton = new System.Windows.Forms.Button();
            this.acercaDeLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(88, 136);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(84, 36);
            this.aceptarButton.TabIndex = 0;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // acercaDeLabel
            // 
            this.acercaDeLabel.AutoSize = true;
            this.acercaDeLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.acercaDeLabel.Location = new System.Drawing.Point(24, 9);
            this.acercaDeLabel.Name = "acercaDeLabel";
            this.acercaDeLabel.Size = new System.Drawing.Size(222, 104);
            this.acercaDeLabel.TabIndex = 1;
            this.acercaDeLabel.Text = "Proyecto Fin de Carrera realizado por:\r\n\r\nAntonio José López Sánchez\r\n\r\n\r\nDirecto" +
                "r de proyecto\r\n\r\nFrancisco Javier Gómez Quesada\r\n";
            this.acercaDeLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // AcercaDeForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(268, 184);
            this.Controls.Add(this.acercaDeLabel);
            this.Controls.Add(this.aceptarButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AcercaDeForm";
            this.Text = "Acerca de MOCR";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Label acercaDeLabel;
    }
}