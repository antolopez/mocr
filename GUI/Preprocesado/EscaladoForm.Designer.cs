namespace OCR.Preprocesado
{
    partial class EscaladoForm
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
            this.proporcionesCheckBox = new System.Windows.Forms.CheckBox();
            this.altoNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.anchoNumericUpDown = new System.Windows.Forms.NumericUpDown();
            this.pixelesLabel1 = new System.Windows.Forms.Label();
            this.pixelesLabel = new System.Windows.Forms.Label();
            this.anchoLabel = new System.Windows.Forms.Label();
            this.altoLabel = new System.Windows.Forms.Label();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.dimensionesGroupBox = new System.Windows.Forms.GroupBox();
            this.escaladoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.altoNumericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.anchoNumericUpDown)).BeginInit();
            this.dimensionesGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // proporcionesCheckBox
            // 
            this.proporcionesCheckBox.AutoSize = true;
            this.proporcionesCheckBox.Checked = true;
            this.proporcionesCheckBox.CheckState = System.Windows.Forms.CheckState.Checked;
            this.proporcionesCheckBox.Location = new System.Drawing.Point(60, 90);
            this.proporcionesCheckBox.Name = "proporcionesCheckBox";
            this.proporcionesCheckBox.Size = new System.Drawing.Size(135, 17);
            this.proporcionesCheckBox.TabIndex = 0;
            this.proporcionesCheckBox.Text = "Mantener proporciones";
            this.proporcionesCheckBox.UseVisualStyleBackColor = true;
            // 
            // altoNumericUpDown
            // 
            this.altoNumericUpDown.Location = new System.Drawing.Point(57, 55);
            this.altoNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.altoNumericUpDown.Name = "altoNumericUpDown";
            this.altoNumericUpDown.Size = new System.Drawing.Size(102, 20);
            this.altoNumericUpDown.TabIndex = 6;
            this.altoNumericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.altoNumericUpDown.ValueChanged += new System.EventHandler(this.altoNumericUpDown_ValueChanged);
            // 
            // anchoNumericUpDown
            // 
            this.anchoNumericUpDown.Location = new System.Drawing.Point(57, 19);
            this.anchoNumericUpDown.Maximum = new decimal(new int[] {
            4096,
            0,
            0,
            0});
            this.anchoNumericUpDown.Name = "anchoNumericUpDown";
            this.anchoNumericUpDown.Size = new System.Drawing.Size(102, 20);
            this.anchoNumericUpDown.TabIndex = 5;
            this.anchoNumericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.anchoNumericUpDown.ValueChanged += new System.EventHandler(this.anchoNumericUpDown_ValueChanged);
            // 
            // pixelesLabel1
            // 
            this.pixelesLabel1.AutoSize = true;
            this.pixelesLabel1.Location = new System.Drawing.Point(163, 57);
            this.pixelesLabel1.Name = "pixelesLabel1";
            this.pixelesLabel1.Size = new System.Drawing.Size(41, 13);
            this.pixelesLabel1.TabIndex = 8;
            this.pixelesLabel1.Text = "píxeles";
            // 
            // pixelesLabel
            // 
            this.pixelesLabel.AutoSize = true;
            this.pixelesLabel.Location = new System.Drawing.Point(163, 22);
            this.pixelesLabel.Name = "pixelesLabel";
            this.pixelesLabel.Size = new System.Drawing.Size(41, 13);
            this.pixelesLabel.TabIndex = 7;
            this.pixelesLabel.Text = "píxeles";
            // 
            // anchoLabel
            // 
            this.anchoLabel.AutoSize = true;
            this.anchoLabel.Location = new System.Drawing.Point(10, 22);
            this.anchoLabel.Name = "anchoLabel";
            this.anchoLabel.Size = new System.Drawing.Size(41, 13);
            this.anchoLabel.TabIndex = 9;
            this.anchoLabel.Text = "Ancho:";
            // 
            // altoLabel
            // 
            this.altoLabel.AutoSize = true;
            this.altoLabel.Location = new System.Drawing.Point(23, 57);
            this.altoLabel.Name = "altoLabel";
            this.altoLabel.Size = new System.Drawing.Size(28, 13);
            this.altoLabel.TabIndex = 10;
            this.altoLabel.Text = "Alto:";
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(60, 114);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 11;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(26, 140);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 12;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.Location = new System.Drawing.Point(153, 140);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 13;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // dimensionesGroupBox
            // 
            this.dimensionesGroupBox.Controls.Add(this.anchoNumericUpDown);
            this.dimensionesGroupBox.Controls.Add(this.pixelesLabel);
            this.dimensionesGroupBox.Controls.Add(this.pixelesLabel1);
            this.dimensionesGroupBox.Controls.Add(this.altoNumericUpDown);
            this.dimensionesGroupBox.Controls.Add(this.altoLabel);
            this.dimensionesGroupBox.Controls.Add(this.anchoLabel);
            this.dimensionesGroupBox.Location = new System.Drawing.Point(15, 3);
            this.dimensionesGroupBox.Name = "dimensionesGroupBox";
            this.dimensionesGroupBox.Size = new System.Drawing.Size(225, 80);
            this.dimensionesGroupBox.TabIndex = 14;
            this.dimensionesGroupBox.TabStop = false;
            this.dimensionesGroupBox.Text = "Dimensiones";
            // 
            // escaladoBackgroundWorker
            // 
            this.escaladoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.escaladoBackgroundWorker_DoWork);
            this.escaladoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.escaladoBackgroundWorker_RunWorkerCompleted);
            // 
            // EscaladoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(255, 172);
            this.Controls.Add(this.dimensionesGroupBox);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.proporcionesCheckBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EscaladoForm";
            this.Text = "Escalado";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EscaladoForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.altoNumericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.anchoNumericUpDown)).EndInit();
            this.dimensionesGroupBox.ResumeLayout(false);
            this.dimensionesGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox proporcionesCheckBox;
        private System.Windows.Forms.NumericUpDown altoNumericUpDown;
        private System.Windows.Forms.NumericUpDown anchoNumericUpDown;
        private System.Windows.Forms.Label pixelesLabel1;
        private System.Windows.Forms.Label pixelesLabel;
        private System.Windows.Forms.Label anchoLabel;
        private System.Windows.Forms.Label altoLabel;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.GroupBox dimensionesGroupBox;
        private System.ComponentModel.BackgroundWorker escaladoBackgroundWorker;
    }
}