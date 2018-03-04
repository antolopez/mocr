namespace OCR.Segmentación
{
    partial class SlantForm
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
            this.distanciaMinimaLabel = new System.Windows.Forms.Label();
            this.distanciaMinimaNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.corregirSlantBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.distanciaMinimaNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(131, 88);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 9;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(17, 88);
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
            this.previsualizarCheckBox.Location = new System.Drawing.Point(68, 57);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 7;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // distanciaMinimaLabel
            // 
            this.distanciaMinimaLabel.AutoSize = true;
            this.distanciaMinimaLabel.Location = new System.Drawing.Point(14, 26);
            this.distanciaMinimaLabel.Name = "distanciaMinimaLabel";
            this.distanciaMinimaLabel.Size = new System.Drawing.Size(91, 13);
            this.distanciaMinimaLabel.TabIndex = 6;
            this.distanciaMinimaLabel.Text = "Distancia mínima:";
            // 
            // distanciaMinimaNumericUpDown
            // 
            this.distanciaMinimaNumericUpDown.Location = new System.Drawing.Point(111, 24);
            this.distanciaMinimaNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.distanciaMinimaNumericUpDown.Name = "distanciaMinimaNumericUpDown";
            this.distanciaMinimaNumericUpDown.Size = new System.Drawing.Size(84, 20);
            this.distanciaMinimaNumericUpDown.TabIndex = 5;
            this.distanciaMinimaNumericUpDown.ValueChanged += new System.EventHandler(this.distanciaMinimaNumericUpDown_ValueChanged);
            // 
            // corregirSlantBackgroundWorker
            // 
            this.corregirSlantBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.corregirSlantBackgroundWorker_DoWork);
            this.corregirSlantBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.corregirSlantBackgroundWorker_RunWorkerCompleted);
            // 
            // SlantForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(219, 124);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.distanciaMinimaLabel);
            this.Controls.Add(this.distanciaMinimaNumericUpDown);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SlantForm";
            this.Text = "Corregir Slant";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.SlantForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.distanciaMinimaNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Label distanciaMinimaLabel;
        private System.Windows.Forms.NumericUpDown distanciaMinimaNumericUpDown;
        private System.ComponentModel.BackgroundWorker corregirSlantBackgroundWorker;
    }
}