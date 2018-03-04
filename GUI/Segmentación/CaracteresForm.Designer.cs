namespace OCR.Segmentación
{
    partial class CaracteresForm
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
            this.incrementoMinimoGroupBox = new System.Windows.Forms.GroupBox();
            this.incrementoMinimoNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.pixelesLabel = new System.Windows.Forms.Label();
            this.grosorTrazoNumericicUpDown = new System.Windows.Forms.NumericUpDown();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.grosorTrazoGroupBox = new System.Windows.Forms.GroupBox();
            this.obtenerCaracteresBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.incrementoMinimoGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.incrementoMinimoNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.grosorTrazoNumericicUpDown)).BeginInit();
            this.grosorTrazoGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // incrementoMinimoGroupBox
            // 
            this.incrementoMinimoGroupBox.Controls.Add(this.incrementoMinimoNumericUpDown);
            this.incrementoMinimoGroupBox.Location = new System.Drawing.Point(11, 76);
            this.incrementoMinimoGroupBox.Name = "incrementoMinimoGroupBox";
            this.incrementoMinimoGroupBox.Size = new System.Drawing.Size(127, 68);
            this.incrementoMinimoGroupBox.TabIndex = 9;
            this.incrementoMinimoGroupBox.TabStop = false;
            this.incrementoMinimoGroupBox.Text = "Selección del incremento mínimo";
            // 
            // incrementoMinimoNumericUpDown
            // 
            this.incrementoMinimoNumericUpDown.DecimalPlaces = 2;
            this.incrementoMinimoNumericUpDown.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.incrementoMinimoNumericUpDown.Location = new System.Drawing.Point(18, 34);
            this.incrementoMinimoNumericUpDown.Name = "incrementoMinimoNumericUpDown";
            this.incrementoMinimoNumericUpDown.Size = new System.Drawing.Size(80, 20);
            this.incrementoMinimoNumericUpDown.TabIndex = 10;
            this.incrementoMinimoNumericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.incrementoMinimoNumericUpDown.ValueChanged += new System.EventHandler(this.incrementoMinimoNumericUpDown_ValueChanged);
            // 
            // pixelesLabel
            // 
            this.pixelesLabel.AutoSize = true;
            this.pixelesLabel.Location = new System.Drawing.Point(147, 26);
            this.pixelesLabel.Name = "pixelesLabel";
            this.pixelesLabel.Size = new System.Drawing.Size(41, 13);
            this.pixelesLabel.TabIndex = 1;
            this.pixelesLabel.Text = "píxeles";
            // 
            // grosorTrazoNumericicUpDown
            // 
            this.grosorTrazoNumericicUpDown.Location = new System.Drawing.Point(18, 24);
            this.grosorTrazoNumericicUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.grosorTrazoNumericicUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.grosorTrazoNumericicUpDown.Name = "grosorTrazoNumericicUpDown";
            this.grosorTrazoNumericicUpDown.Size = new System.Drawing.Size(109, 20);
            this.grosorTrazoNumericicUpDown.TabIndex = 0;
            this.grosorTrazoNumericicUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.grosorTrazoNumericicUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.grosorTrazoNumericicUpDown.ValueChanged += new System.EventHandler(this.grosorTrazoNumericicUpDown_ValueChanged);
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(151, 103);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 7;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(144, 162);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 6;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(31, 162);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 5;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // grosorTrazoGroupBox
            // 
            this.grosorTrazoGroupBox.Controls.Add(this.pixelesLabel);
            this.grosorTrazoGroupBox.Controls.Add(this.grosorTrazoNumericicUpDown);
            this.grosorTrazoGroupBox.Location = new System.Drawing.Point(11, 8);
            this.grosorTrazoGroupBox.Name = "grosorTrazoGroupBox";
            this.grosorTrazoGroupBox.Size = new System.Drawing.Size(225, 57);
            this.grosorTrazoGroupBox.TabIndex = 10;
            this.grosorTrazoGroupBox.TabStop = false;
            this.grosorTrazoGroupBox.Text = "Selección del grosor del trazo";
            // 
            // obtenerCaracteresBackgroundWorker
            // 
            this.obtenerCaracteresBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.obtenerCaracteresBackgroundWorker_DoWork);
            this.obtenerCaracteresBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.obtenerCaracteresBackgroundWorker_RunWorkerCompleted);
            // 
            // CaracteresForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(251, 197);
            this.Controls.Add(this.grosorTrazoGroupBox);
            this.Controls.Add(this.incrementoMinimoGroupBox);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CaracteresForm";
            this.Text = "Segmentación de caracteres";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.CaracteresForm_FormClosing);
            this.incrementoMinimoGroupBox.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.incrementoMinimoNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.grosorTrazoNumericicUpDown)).EndInit();
            this.grosorTrazoGroupBox.ResumeLayout(false);
            this.grosorTrazoGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox incrementoMinimoGroupBox;
        private System.Windows.Forms.Label pixelesLabel;
        private System.Windows.Forms.NumericUpDown grosorTrazoNumericicUpDown;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.NumericUpDown incrementoMinimoNumericUpDown;
        private System.Windows.Forms.GroupBox grosorTrazoGroupBox;
        private System.ComponentModel.BackgroundWorker obtenerCaracteresBackgroundWorker;
    }
}