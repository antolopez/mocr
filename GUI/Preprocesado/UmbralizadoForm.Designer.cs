namespace OCR.Preprocesado
{
    partial class UmbralizadoForm
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
            this.umbralTrackBar = new System.Windows.Forms.TrackBar();
            this.umbralTextBox = new System.Windows.Forms.TextBox();
            this.label0 = new System.Windows.Forms.Label();
            this.label255 = new System.Windows.Forms.Label();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.umbralLabel = new System.Windows.Forms.Label();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.seleccionUmbralGroupBox = new System.Windows.Forms.GroupBox();
            this.umbralizadoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.umbralTrackBar)).BeginInit();
            this.seleccionUmbralGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // umbralTrackBar
            // 
            this.umbralTrackBar.LargeChange = 10;
            this.umbralTrackBar.Location = new System.Drawing.Point(18, 48);
            this.umbralTrackBar.Maximum = 255;
            this.umbralTrackBar.Name = "umbralTrackBar";
            this.umbralTrackBar.Size = new System.Drawing.Size(194, 45);
            this.umbralTrackBar.TabIndex = 0;
            this.umbralTrackBar.TickFrequency = 10;
            this.umbralTrackBar.Scroll += new System.EventHandler(this.umbralTrackBar_Scroll);
            // 
            // umbralTextBox
            // 
            this.umbralTextBox.Location = new System.Drawing.Point(109, 19);
            this.umbralTextBox.Name = "umbralTextBox";
            this.umbralTextBox.ReadOnly = true;
            this.umbralTextBox.Size = new System.Drawing.Size(70, 20);
            this.umbralTextBox.TabIndex = 1;
            this.umbralTextBox.Text = "0";
            this.umbralTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.umbralTextBox.TextChanged += new System.EventHandler(this.umbralTextBox_TextChanged);
            // 
            // label0
            // 
            this.label0.AutoSize = true;
            this.label0.Location = new System.Drawing.Point(9, 53);
            this.label0.Name = "label0";
            this.label0.Size = new System.Drawing.Size(13, 13);
            this.label0.TabIndex = 2;
            this.label0.Text = "0";
            // 
            // label255
            // 
            this.label255.AutoSize = true;
            this.label255.Location = new System.Drawing.Point(209, 53);
            this.label255.Name = "label255";
            this.label255.Size = new System.Drawing.Size(25, 13);
            this.label255.TabIndex = 2;
            this.label255.Text = "255";
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(87, 112);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 3;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // umbralLabel
            // 
            this.umbralLabel.AutoSize = true;
            this.umbralLabel.Location = new System.Drawing.Point(60, 22);
            this.umbralLabel.Name = "umbralLabel";
            this.umbralLabel.Size = new System.Drawing.Size(43, 13);
            this.umbralLabel.TabIndex = 4;
            this.umbralLabel.Text = "Umbral:";
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(22, 140);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 5;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(165, 140);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 6;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // seleccionUmbralGroupBox
            // 
            this.seleccionUmbralGroupBox.Controls.Add(this.umbralTextBox);
            this.seleccionUmbralGroupBox.Controls.Add(this.umbralTrackBar);
            this.seleccionUmbralGroupBox.Controls.Add(this.label0);
            this.seleccionUmbralGroupBox.Controls.Add(this.umbralLabel);
            this.seleccionUmbralGroupBox.Controls.Add(this.label255);
            this.seleccionUmbralGroupBox.Location = new System.Drawing.Point(8, 4);
            this.seleccionUmbralGroupBox.Name = "seleccionUmbralGroupBox";
            this.seleccionUmbralGroupBox.Size = new System.Drawing.Size(242, 100);
            this.seleccionUmbralGroupBox.TabIndex = 7;
            this.seleccionUmbralGroupBox.TabStop = false;
            this.seleccionUmbralGroupBox.Text = "Selección del umbral";
            // 
            // umbralizadoBackgroundWorker
            // 
            this.umbralizadoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.umbralizadoBackgroundWorker_DoWork);
            this.umbralizadoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.umbralizadoBackgroundWorker_RunWorkerCompleted);
            // 
            // UmbralizadoForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(258, 173);
            this.Controls.Add(this.seleccionUmbralGroupBox);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.previsualizarCheckBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "UmbralizadoForm";
            this.Text = "Umbralizado";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.UmbralizadoForm_FormClosing);
            ((System.ComponentModel.ISupportInitialize)(this.umbralTrackBar)).EndInit();
            this.seleccionUmbralGroupBox.ResumeLayout(false);
            this.seleccionUmbralGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar umbralTrackBar;
        private System.Windows.Forms.TextBox umbralTextBox;
        private System.Windows.Forms.Label label0;
        private System.Windows.Forms.Label label255;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Label umbralLabel;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.Windows.Forms.GroupBox seleccionUmbralGroupBox;
        private System.ComponentModel.BackgroundWorker umbralizadoBackgroundWorker;
    }
}