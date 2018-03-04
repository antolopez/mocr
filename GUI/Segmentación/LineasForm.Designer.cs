namespace OCR.Segmentación
{
    partial class LineasForm
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
            this.cancelarButton = new System.Windows.Forms.Button();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.interlineadoMedioGroupBox = new System.Windows.Forms.GroupBox();
            this.pixelesLabel = new System.Windows.Forms.Label();
            this.interlineadoMedioNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.incrementoMinimoGroupBox = new System.Windows.Forms.GroupBox();
            this.pixelesLabel1 = new System.Windows.Forms.Label();
            this.incrementoMinimoNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.obtenerLineasBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.interlineadoMedioGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.interlineadoMedioNumericUpDown)).BeginInit();
            this.incrementoMinimoGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.incrementoMinimoNumericUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(29, 158);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 0;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.Location = new System.Drawing.Point(134, 158);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 1;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(145, 100);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 2;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // interlineadoMedioGroupBox
            // 
            this.interlineadoMedioGroupBox.Controls.Add(this.pixelesLabel);
            this.interlineadoMedioGroupBox.Controls.Add(this.interlineadoMedioNumericUpDown);
            this.interlineadoMedioGroupBox.Location = new System.Drawing.Point(12, 12);
            this.interlineadoMedioGroupBox.Name = "interlineadoMedioGroupBox";
            this.interlineadoMedioGroupBox.Size = new System.Drawing.Size(215, 55);
            this.interlineadoMedioGroupBox.TabIndex = 3;
            this.interlineadoMedioGroupBox.TabStop = false;
            this.interlineadoMedioGroupBox.Text = "Selección del interlineado medio";
            // 
            // pixelesLabel
            // 
            this.pixelesLabel.AutoSize = true;
            this.pixelesLabel.Location = new System.Drawing.Point(146, 23);
            this.pixelesLabel.Name = "pixelesLabel";
            this.pixelesLabel.Size = new System.Drawing.Size(41, 13);
            this.pixelesLabel.TabIndex = 1;
            this.pixelesLabel.Text = "píxeles";
            // 
            // interlineadoMedioNumericUpDown
            // 
            this.interlineadoMedioNumericUpDown.Location = new System.Drawing.Point(24, 21);
            this.interlineadoMedioNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.interlineadoMedioNumericUpDown.Name = "interlineadoMedioNumericUpDown";
            this.interlineadoMedioNumericUpDown.Size = new System.Drawing.Size(116, 20);
            this.interlineadoMedioNumericUpDown.TabIndex = 0;
            this.interlineadoMedioNumericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.interlineadoMedioNumericUpDown.ValueChanged += new System.EventHandler(this.interlineadoMedioNumericUpDown_ValueChanged);
            // 
            // incrementoMinimoGroupBox
            // 
            this.incrementoMinimoGroupBox.Controls.Add(this.pixelesLabel1);
            this.incrementoMinimoGroupBox.Controls.Add(this.incrementoMinimoNumericUpDown);
            this.incrementoMinimoGroupBox.Location = new System.Drawing.Point(12, 73);
            this.incrementoMinimoGroupBox.Name = "incrementoMinimoGroupBox";
            this.incrementoMinimoGroupBox.Size = new System.Drawing.Size(127, 68);
            this.incrementoMinimoGroupBox.TabIndex = 4;
            this.incrementoMinimoGroupBox.TabStop = false;
            this.incrementoMinimoGroupBox.Text = "Selección del incremento mínimo";
            // 
            // pixelesLabel1
            // 
            this.pixelesLabel1.AutoSize = true;
            this.pixelesLabel1.Location = new System.Drawing.Point(81, 37);
            this.pixelesLabel1.Name = "pixelesLabel1";
            this.pixelesLabel1.Size = new System.Drawing.Size(41, 13);
            this.pixelesLabel1.TabIndex = 1;
            this.pixelesLabel1.Text = "píxeles";
            // 
            // incrementoMinimoNumericUpDown
            // 
            this.incrementoMinimoNumericUpDown.Location = new System.Drawing.Point(7, 35);
            this.incrementoMinimoNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.incrementoMinimoNumericUpDown.Name = "incrementoMinimoNumericUpDown";
            this.incrementoMinimoNumericUpDown.Size = new System.Drawing.Size(69, 20);
            this.incrementoMinimoNumericUpDown.TabIndex = 0;
            this.incrementoMinimoNumericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.incrementoMinimoNumericUpDown.ValueChanged += new System.EventHandler(this.incrementoMinimoNumericUpDown_ValueChanged);
            // 
            // obtenerLineasBackgroundWorker
            // 
            this.obtenerLineasBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.obtenerLineasBackgroundWorker_DoWork);
            this.obtenerLineasBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.obtenerLineasBackgroundWorker_RunWorkerCompleted);
            // 
            // LineasForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(238, 193);
            this.Controls.Add(this.incrementoMinimoGroupBox);
            this.Controls.Add(this.interlineadoMedioGroupBox);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LineasForm";
            this.Text = "Segmentación de líneas";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.LineasForm_FormClosing);
            this.interlineadoMedioGroupBox.ResumeLayout(false);
            this.interlineadoMedioGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.interlineadoMedioNumericUpDown)).EndInit();
            this.incrementoMinimoGroupBox.ResumeLayout(false);
            this.incrementoMinimoGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.incrementoMinimoNumericUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.GroupBox interlineadoMedioGroupBox;
        private System.Windows.Forms.NumericUpDown interlineadoMedioNumericUpDown;
        private System.Windows.Forms.Label pixelesLabel;
        private System.Windows.Forms.GroupBox incrementoMinimoGroupBox;
        private System.Windows.Forms.Label pixelesLabel1;
        private System.Windows.Forms.NumericUpDown incrementoMinimoNumericUpDown;
        private System.ComponentModel.BackgroundWorker obtenerLineasBackgroundWorker;
    }
}