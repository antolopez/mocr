namespace OCR
{
    partial class OpcionesKNNForm
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
            this.numeroVecinosNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.numeroVecinosLabel = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numeroVecinosNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(66, 59);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 0;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // numeroVecinosNumericUpDown
            // 
            this.numeroVecinosNumericUpDown.Location = new System.Drawing.Point(95, 23);
            this.numeroVecinosNumericUpDown.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numeroVecinosNumericUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numeroVecinosNumericUpDown.Name = "numeroVecinosNumericUpDown";
            this.numeroVecinosNumericUpDown.Size = new System.Drawing.Size(85, 20);
            this.numeroVecinosNumericUpDown.TabIndex = 1;
            this.numeroVecinosNumericUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // numeroVecinosLabel
            // 
            this.numeroVecinosLabel.AutoSize = true;
            this.numeroVecinosLabel.Location = new System.Drawing.Point(12, 25);
            this.numeroVecinosLabel.Name = "numeroVecinosLabel";
            this.numeroVecinosLabel.Size = new System.Drawing.Size(77, 13);
            this.numeroVecinosLabel.TabIndex = 2;
            this.numeroVecinosLabel.Text = "Nº de vecinos:";
            // 
            // OpcionesKNNForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(199, 94);
            this.Controls.Add(this.numeroVecinosLabel);
            this.Controls.Add(this.numeroVecinosNumericUpDown);
            this.Controls.Add(this.aceptarButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "OpcionesKNNForm";
            this.Text = "OpcionesKNNForm";
            ((System.ComponentModel.ISupportInitialize)(this.numeroVecinosNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.NumericUpDown numeroVecinosNumericUpDown;
        private System.Windows.Forms.Label numeroVecinosLabel;
    }
}