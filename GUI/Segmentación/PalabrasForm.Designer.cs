namespace OCR.Segmentación
{
    partial class PalabrasForm
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
            this.cancelarButton = new System.Windows.Forms.Button();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.espaciadoMedioLabel = new System.Windows.Forms.Label();
            this.espaciadoMedioNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.obtenerPalabrasBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.espaciadoMedioNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(139, 81);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 9;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(25, 81);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 8;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(76, 50);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 7;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // espaciadoMedioLabel
            // 
            this.espaciadoMedioLabel.AutoSize = true;
            this.espaciadoMedioLabel.Location = new System.Drawing.Point(22, 19);
            this.espaciadoMedioLabel.Name = "espaciadoMedioLabel";
            this.espaciadoMedioLabel.Size = new System.Drawing.Size(91, 13);
            this.espaciadoMedioLabel.TabIndex = 6;
            this.espaciadoMedioLabel.Text = "Espaciado medio:";
            // 
            // espaciadoMedioNumericUpDown
            // 
            this.espaciadoMedioNumericUpDown.Location = new System.Drawing.Point(119, 17);
            this.espaciadoMedioNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.espaciadoMedioNumericUpDown.Name = "espaciadoMedioNumericUpDown";
            this.espaciadoMedioNumericUpDown.Size = new System.Drawing.Size(84, 20);
            this.espaciadoMedioNumericUpDown.TabIndex = 5;
            this.espaciadoMedioNumericUpDown.ValueChanged += new System.EventHandler(this.espaciadoMedioNumericUpDown_ValueChanged);
            // 
            // obtenerPalabrasBackgroundWorker
            // 
            this.obtenerPalabrasBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.obtenerPalabrasBackgroundWorker_DoWork);
            this.obtenerPalabrasBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.obtenerPalabrasBackgroundWorker_RunWorkerCompleted);
            // 
            // PalabrasForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(237, 113);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.espaciadoMedioLabel);
            this.Controls.Add(this.espaciadoMedioNumericUpDown);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "PalabrasForm";
            this.Text = "Segmentación de palabras";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.PalabrasForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.espaciadoMedioNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Label espaciadoMedioLabel;
        private System.Windows.Forms.NumericUpDown espaciadoMedioNumericUpDown;
        private System.ComponentModel.BackgroundWorker obtenerPalabrasBackgroundWorker;
    }
}