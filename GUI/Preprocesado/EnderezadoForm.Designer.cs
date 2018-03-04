namespace OCR.Preprocesado
{
    partial class EnderezadoForm
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
            this.direccionGroupBox = new System.Windows.Forms.GroupBox();
            this.izquierdaRadioButton = new System.Windows.Forms.RadioButton();
            this.derechaRadioButton = new System.Windows.Forms.RadioButton();
            this.gradosGroupBox = new System.Windows.Forms.GroupBox();
            this.label360 = new System.Windows.Forms.Label();
            this.label0 = new System.Windows.Forms.Label();
            this.gradosTextBox = new System.Windows.Forms.TextBox();
            this.gradosTrackBar = new System.Windows.Forms.TrackBar();
            this.modoGroupBox = new System.Windows.Forms.GroupBox();
            this.automaticoRadioButton = new System.Windows.Forms.RadioButton();
            this.manualRadioButton = new System.Windows.Forms.RadioButton();
            this.previsualizarCheckBox = new System.Windows.Forms.CheckBox();
            this.aceptarButton = new System.Windows.Forms.Button();
            this.cancelarButton = new System.Windows.Forms.Button();
            this.enderezadoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.direccionGroupBox.SuspendLayout();
            this.gradosGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gradosTrackBar)).BeginInit();
            this.modoGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // direccionGroupBox
            // 
            this.direccionGroupBox.Controls.Add(this.izquierdaRadioButton);
            this.direccionGroupBox.Controls.Add(this.derechaRadioButton);
            this.direccionGroupBox.Location = new System.Drawing.Point(12, 71);
            this.direccionGroupBox.Name = "direccionGroupBox";
            this.direccionGroupBox.Size = new System.Drawing.Size(106, 87);
            this.direccionGroupBox.TabIndex = 2;
            this.direccionGroupBox.TabStop = false;
            this.direccionGroupBox.Text = "Dirección";
            // 
            // izquierdaRadioButton
            // 
            this.izquierdaRadioButton.AutoSize = true;
            this.izquierdaRadioButton.Location = new System.Drawing.Point(21, 52);
            this.izquierdaRadioButton.Name = "izquierdaRadioButton";
            this.izquierdaRadioButton.Size = new System.Drawing.Size(68, 17);
            this.izquierdaRadioButton.TabIndex = 1;
            this.izquierdaRadioButton.Text = "Izquierda";
            this.izquierdaRadioButton.UseVisualStyleBackColor = true;
            this.izquierdaRadioButton.CheckedChanged += new System.EventHandler(this.izquierdaRadioButton_CheckedChanged);
            // 
            // derechaRadioButton
            // 
            this.derechaRadioButton.AutoSize = true;
            this.derechaRadioButton.Checked = true;
            this.derechaRadioButton.Location = new System.Drawing.Point(21, 29);
            this.derechaRadioButton.Name = "derechaRadioButton";
            this.derechaRadioButton.Size = new System.Drawing.Size(66, 17);
            this.derechaRadioButton.TabIndex = 0;
            this.derechaRadioButton.TabStop = true;
            this.derechaRadioButton.Text = "Derecha";
            this.derechaRadioButton.UseVisualStyleBackColor = true;
            this.derechaRadioButton.CheckedChanged += new System.EventHandler(this.derechaRadioButton_CheckedChanged);
            // 
            // gradosGroupBox
            // 
            this.gradosGroupBox.Controls.Add(this.label360);
            this.gradosGroupBox.Controls.Add(this.label0);
            this.gradosGroupBox.Controls.Add(this.gradosTextBox);
            this.gradosGroupBox.Controls.Add(this.gradosTrackBar);
            this.gradosGroupBox.Location = new System.Drawing.Point(125, 71);
            this.gradosGroupBox.Name = "gradosGroupBox";
            this.gradosGroupBox.Size = new System.Drawing.Size(155, 87);
            this.gradosGroupBox.TabIndex = 3;
            this.gradosGroupBox.TabStop = false;
            this.gradosGroupBox.Text = "Grados";
            // 
            // label360
            // 
            this.label360.AutoSize = true;
            this.label360.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label360.Location = new System.Drawing.Point(115, 45);
            this.label360.Name = "label360";
            this.label360.Size = new System.Drawing.Size(32, 15);
            this.label360.TabIndex = 4;
            this.label360.Text = "360º";
            // 
            // label0
            // 
            this.label0.AutoSize = true;
            this.label0.Font = new System.Drawing.Font("Arial", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label0.Location = new System.Drawing.Point(25, 45);
            this.label0.Name = "label0";
            this.label0.Size = new System.Drawing.Size(18, 15);
            this.label0.TabIndex = 4;
            this.label0.Text = "0º";
            // 
            // gradosTextBox
            // 
            this.gradosTextBox.Location = new System.Drawing.Point(55, 62);
            this.gradosTextBox.Name = "gradosTextBox";
            this.gradosTextBox.ReadOnly = true;
            this.gradosTextBox.Size = new System.Drawing.Size(44, 20);
            this.gradosTextBox.TabIndex = 3;
            this.gradosTextBox.Text = "0";
            this.gradosTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // gradosTrackBar
            // 
            this.gradosTrackBar.LargeChange = 10;
            this.gradosTrackBar.Location = new System.Drawing.Point(19, 17);
            this.gradosTrackBar.Maximum = 255;
            this.gradosTrackBar.Name = "gradosTrackBar";
            this.gradosTrackBar.Size = new System.Drawing.Size(121, 45);
            this.gradosTrackBar.TabIndex = 2;
            this.gradosTrackBar.TickFrequency = 15;
            this.gradosTrackBar.Scroll += new System.EventHandler(this.gradosTrackBar_Scroll);
            // 
            // modoGroupBox
            // 
            this.modoGroupBox.Controls.Add(this.automaticoRadioButton);
            this.modoGroupBox.Controls.Add(this.manualRadioButton);
            this.modoGroupBox.Location = new System.Drawing.Point(12, 2);
            this.modoGroupBox.Name = "modoGroupBox";
            this.modoGroupBox.Size = new System.Drawing.Size(106, 63);
            this.modoGroupBox.TabIndex = 4;
            this.modoGroupBox.TabStop = false;
            this.modoGroupBox.Text = "Modo";
            // 
            // automaticoRadioButton
            // 
            this.automaticoRadioButton.AutoSize = true;
            this.automaticoRadioButton.Location = new System.Drawing.Point(14, 40);
            this.automaticoRadioButton.Name = "automaticoRadioButton";
            this.automaticoRadioButton.Size = new System.Drawing.Size(78, 17);
            this.automaticoRadioButton.TabIndex = 1;
            this.automaticoRadioButton.Text = "Automático";
            this.automaticoRadioButton.UseVisualStyleBackColor = true;
            this.automaticoRadioButton.CheckedChanged += new System.EventHandler(this.automaticoRadioButton_CheckedChanged);
            // 
            // manualRadioButton
            // 
            this.manualRadioButton.AutoSize = true;
            this.manualRadioButton.Checked = true;
            this.manualRadioButton.Location = new System.Drawing.Point(14, 19);
            this.manualRadioButton.Name = "manualRadioButton";
            this.manualRadioButton.Size = new System.Drawing.Size(60, 17);
            this.manualRadioButton.TabIndex = 0;
            this.manualRadioButton.TabStop = true;
            this.manualRadioButton.Text = "Manual";
            this.manualRadioButton.UseVisualStyleBackColor = true;
            this.manualRadioButton.CheckedChanged += new System.EventHandler(this.manualRadioButton_CheckedChanged);
            // 
            // previsualizarCheckBox
            // 
            this.previsualizarCheckBox.AutoSize = true;
            this.previsualizarCheckBox.Location = new System.Drawing.Point(165, 31);
            this.previsualizarCheckBox.Name = "previsualizarCheckBox";
            this.previsualizarCheckBox.Size = new System.Drawing.Size(85, 17);
            this.previsualizarCheckBox.TabIndex = 5;
            this.previsualizarCheckBox.Text = "Previsualizar";
            this.previsualizarCheckBox.UseVisualStyleBackColor = true;
            this.previsualizarCheckBox.CheckedChanged += new System.EventHandler(this.previsualizarCheckBox_CheckedChanged);
            // 
            // aceptarButton
            // 
            this.aceptarButton.Location = new System.Drawing.Point(48, 167);
            this.aceptarButton.Name = "aceptarButton";
            this.aceptarButton.Size = new System.Drawing.Size(75, 23);
            this.aceptarButton.TabIndex = 6;
            this.aceptarButton.Text = "Aceptar";
            this.aceptarButton.UseVisualStyleBackColor = true;
            this.aceptarButton.Click += new System.EventHandler(this.aceptarButton_Click);
            // 
            // cancelarButton
            // 
            this.cancelarButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelarButton.Location = new System.Drawing.Point(170, 167);
            this.cancelarButton.Name = "cancelarButton";
            this.cancelarButton.Size = new System.Drawing.Size(75, 23);
            this.cancelarButton.TabIndex = 7;
            this.cancelarButton.Text = "Cancelar";
            this.cancelarButton.UseVisualStyleBackColor = true;
            this.cancelarButton.Click += new System.EventHandler(this.cancelarButton_Click);
            // 
            // enderezadoBackgroundWorker
            // 
            this.enderezadoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.enderezadoBackgroundWorker_DoWork);
            this.enderezadoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.enderezadoBackgroundWorker_RunWorkerCompleted);
            // 
            // EnderezadoForm
            // 
            this.AcceptButton = this.aceptarButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelarButton;
            this.ClientSize = new System.Drawing.Size(292, 200);
            this.Controls.Add(this.cancelarButton);
            this.Controls.Add(this.aceptarButton);
            this.Controls.Add(this.previsualizarCheckBox);
            this.Controls.Add(this.modoGroupBox);
            this.Controls.Add(this.gradosGroupBox);
            this.Controls.Add(this.direccionGroupBox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "EnderezadoForm";
            this.Text = "Enderezado";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EnderezadoForm_FormClosing);
            this.direccionGroupBox.ResumeLayout(false);
            this.direccionGroupBox.PerformLayout();
            this.gradosGroupBox.ResumeLayout(false);
            this.gradosGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.gradosTrackBar)).EndInit();
            this.modoGroupBox.ResumeLayout(false);
            this.modoGroupBox.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox direccionGroupBox;
        private System.Windows.Forms.RadioButton izquierdaRadioButton;
        private System.Windows.Forms.RadioButton derechaRadioButton;
        private System.Windows.Forms.GroupBox gradosGroupBox;
        private System.Windows.Forms.TextBox gradosTextBox;
        private System.Windows.Forms.TrackBar gradosTrackBar;
        private System.Windows.Forms.Label label360;
        private System.Windows.Forms.Label label0;
        private System.Windows.Forms.GroupBox modoGroupBox;
        private System.Windows.Forms.RadioButton automaticoRadioButton;
        private System.Windows.Forms.RadioButton manualRadioButton;
        private System.Windows.Forms.CheckBox previsualizarCheckBox;
        private System.Windows.Forms.Button aceptarButton;
        private System.Windows.Forms.Button cancelarButton;
        private System.ComponentModel.BackgroundWorker enderezadoBackgroundWorker;
    }
}