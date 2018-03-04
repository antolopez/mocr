namespace OCR
{
    partial class TextoReconocidoForm
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
            this.textoReconocidoRichTextBox = new System.Windows.Forms.RichTextBox();
            this.ejecutarButton = new System.Windows.Forms.Button();
            this.textoLabel = new System.Windows.Forms.Label();
            this.aprendizajeRadioButton = new System.Windows.Forms.RadioButton();
            this.reconocimientoRadioButton = new System.Windows.Forms.RadioButton();
            this.modoGroupBox = new System.Windows.Forms.GroupBox();
            this.clasificadorGroupBox = new System.Windows.Forms.GroupBox();
            this.clasificadorComboBox = new System.Windows.Forms.ComboBox();
            this.configuracionButton = new System.Windows.Forms.Button();
            this.ejecutarProgressBar = new System.Windows.Forms.ProgressBar();
            this.corregirButton = new System.Windows.Forms.Button();
            this.exportarButton = new System.Windows.Forms.Button();
            this.exportarTextoSaveFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.ejecutarBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.corregirBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.modoGroupBox.SuspendLayout();
            this.clasificadorGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // textoReconocidoRichTextBox
            // 
            this.textoReconocidoRichTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textoReconocidoRichTextBox.Location = new System.Drawing.Point(15, 50);
            this.textoReconocidoRichTextBox.Name = "textoReconocidoRichTextBox";
            this.textoReconocidoRichTextBox.Size = new System.Drawing.Size(402, 412);
            this.textoReconocidoRichTextBox.TabIndex = 0;
            this.textoReconocidoRichTextBox.Text = "";
            this.textoReconocidoRichTextBox.TextChanged += new System.EventHandler(this.textoReconocidoRichTextBox_TextChanged);
            // 
            // ejecutarButton
            // 
            this.ejecutarButton.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.ejecutarButton.Location = new System.Drawing.Point(436, 240);
            this.ejecutarButton.Name = "ejecutarButton";
            this.ejecutarButton.Size = new System.Drawing.Size(82, 57);
            this.ejecutarButton.TabIndex = 1;
            this.ejecutarButton.Text = "Ejecutar";
            this.ejecutarButton.UseVisualStyleBackColor = true;
            this.ejecutarButton.Click += new System.EventHandler(this.ejecutarButton_Click);
            // 
            // textoLabel
            // 
            this.textoLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textoLabel.AutoSize = true;
            this.textoLabel.Font = new System.Drawing.Font("Comic Sans MS", 21.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textoLabel.Location = new System.Drawing.Point(165, 6);
            this.textoLabel.Name = "textoLabel";
            this.textoLabel.Size = new System.Drawing.Size(108, 40);
            this.textoLabel.TabIndex = 2;
            this.textoLabel.Text = "Texto:";
            // 
            // aprendizajeRadioButton
            // 
            this.aprendizajeRadioButton.AutoSize = true;
            this.aprendizajeRadioButton.Checked = true;
            this.aprendizajeRadioButton.Location = new System.Drawing.Point(19, 19);
            this.aprendizajeRadioButton.Name = "aprendizajeRadioButton";
            this.aprendizajeRadioButton.Size = new System.Drawing.Size(80, 17);
            this.aprendizajeRadioButton.TabIndex = 3;
            this.aprendizajeRadioButton.TabStop = true;
            this.aprendizajeRadioButton.Text = "Aprendizaje";
            this.aprendizajeRadioButton.UseVisualStyleBackColor = true;
            this.aprendizajeRadioButton.CheckedChanged += new System.EventHandler(this.aprendizajeRadioButton_CheckedChanged);
            // 
            // reconocimientoRadioButton
            // 
            this.reconocimientoRadioButton.AutoSize = true;
            this.reconocimientoRadioButton.Location = new System.Drawing.Point(19, 43);
            this.reconocimientoRadioButton.Name = "reconocimientoRadioButton";
            this.reconocimientoRadioButton.Size = new System.Drawing.Size(102, 17);
            this.reconocimientoRadioButton.TabIndex = 4;
            this.reconocimientoRadioButton.Text = "Reconocimiento";
            this.reconocimientoRadioButton.UseVisualStyleBackColor = true;
            this.reconocimientoRadioButton.CheckedChanged += new System.EventHandler(this.reconocimientoRadioButton_CheckedChanged);
            // 
            // modoGroupBox
            // 
            this.modoGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modoGroupBox.Controls.Add(this.reconocimientoRadioButton);
            this.modoGroupBox.Controls.Add(this.aprendizajeRadioButton);
            this.modoGroupBox.Location = new System.Drawing.Point(472, 22);
            this.modoGroupBox.Name = "modoGroupBox";
            this.modoGroupBox.Size = new System.Drawing.Size(135, 72);
            this.modoGroupBox.TabIndex = 5;
            this.modoGroupBox.TabStop = false;
            this.modoGroupBox.Text = "Modo";
            // 
            // clasificadorGroupBox
            // 
            this.clasificadorGroupBox.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.clasificadorGroupBox.Controls.Add(this.clasificadorComboBox);
            this.clasificadorGroupBox.Controls.Add(this.configuracionButton);
            this.clasificadorGroupBox.Enabled = false;
            this.clasificadorGroupBox.Location = new System.Drawing.Point(436, 113);
            this.clasificadorGroupBox.Name = "clasificadorGroupBox";
            this.clasificadorGroupBox.Size = new System.Drawing.Size(200, 100);
            this.clasificadorGroupBox.TabIndex = 6;
            this.clasificadorGroupBox.TabStop = false;
            this.clasificadorGroupBox.Text = "Clasificador";
            // 
            // clasificadorComboBox
            // 
            this.clasificadorComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.clasificadorComboBox.FormattingEnabled = true;
            this.clasificadorComboBox.Items.AddRange(new object[] {
            "KNN",
            "Naive Bayes"});
            this.clasificadorComboBox.Location = new System.Drawing.Point(21, 28);
            this.clasificadorComboBox.Name = "clasificadorComboBox";
            this.clasificadorComboBox.Size = new System.Drawing.Size(156, 21);
            this.clasificadorComboBox.TabIndex = 1;
            // 
            // configuracionButton
            // 
            this.configuracionButton.Location = new System.Drawing.Point(56, 64);
            this.configuracionButton.Name = "configuracionButton";
            this.configuracionButton.Size = new System.Drawing.Size(87, 23);
            this.configuracionButton.TabIndex = 0;
            this.configuracionButton.Text = "Configuración";
            this.configuracionButton.UseVisualStyleBackColor = true;
            this.configuracionButton.Click += new System.EventHandler(this.configuracionButton_Click);
            // 
            // ejecutarProgressBar
            // 
            this.ejecutarProgressBar.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.ejecutarProgressBar.Location = new System.Drawing.Point(532, 255);
            this.ejecutarProgressBar.Name = "ejecutarProgressBar";
            this.ejecutarProgressBar.Size = new System.Drawing.Size(100, 23);
            this.ejecutarProgressBar.Step = 0;
            this.ejecutarProgressBar.TabIndex = 7;
            // 
            // corregirButton
            // 
            this.corregirButton.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.corregirButton.Enabled = false;
            this.corregirButton.Location = new System.Drawing.Point(457, 320);
            this.corregirButton.Name = "corregirButton";
            this.corregirButton.Size = new System.Drawing.Size(156, 52);
            this.corregirButton.TabIndex = 8;
            this.corregirButton.Text = "Corregir";
            this.corregirButton.UseVisualStyleBackColor = true;
            this.corregirButton.Click += new System.EventHandler(this.corregirButton_Click);
            // 
            // exportarButton
            // 
            this.exportarButton.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.exportarButton.Enabled = false;
            this.exportarButton.Location = new System.Drawing.Point(457, 395);
            this.exportarButton.Name = "exportarButton";
            this.exportarButton.Size = new System.Drawing.Size(156, 55);
            this.exportarButton.TabIndex = 9;
            this.exportarButton.Text = "Exportar texto";
            this.exportarButton.UseVisualStyleBackColor = true;
            this.exportarButton.Click += new System.EventHandler(this.exportarButton_Click);
            // 
            // exportarTextoSaveFileDialog
            // 
            this.exportarTextoSaveFileDialog.AddExtension = false;
            this.exportarTextoSaveFileDialog.Title = "Guardar texto";
            this.exportarTextoSaveFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.exportarTextoSaveFileDialog_FileOk);
            // 
            // ejecutarBackgroundWorker
            // 
            this.ejecutarBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.ejecutarBackgroundWorker_DoWork);
            this.ejecutarBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.ejecutarBackgroundWorker_RunWorkerCompleted);
            // 
            // corregirBackgroundWorker
            // 
            this.corregirBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.corregirBackgroundWorker_DoWork);
            this.corregirBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.corregirBackgroundWorker_RunWorkerCompleted);
            // 
            // TextoReconocidoForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(648, 483);
            this.Controls.Add(this.exportarButton);
            this.Controls.Add(this.corregirButton);
            this.Controls.Add(this.ejecutarProgressBar);
            this.Controls.Add(this.clasificadorGroupBox);
            this.Controls.Add(this.modoGroupBox);
            this.Controls.Add(this.textoLabel);
            this.Controls.Add(this.ejecutarButton);
            this.Controls.Add(this.textoReconocidoRichTextBox);
            this.Name = "TextoReconocidoForm";
            this.ShowIcon = false;
            this.Text = "Reconocimiento";
            this.modoGroupBox.ResumeLayout(false);
            this.modoGroupBox.PerformLayout();
            this.clasificadorGroupBox.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox textoReconocidoRichTextBox;
        private System.Windows.Forms.Button ejecutarButton;
        private System.Windows.Forms.Label textoLabel;
        private System.Windows.Forms.RadioButton aprendizajeRadioButton;
        private System.Windows.Forms.RadioButton reconocimientoRadioButton;
        private System.Windows.Forms.GroupBox modoGroupBox;
        private System.Windows.Forms.GroupBox clasificadorGroupBox;
        private System.Windows.Forms.ComboBox clasificadorComboBox;
        private System.Windows.Forms.Button configuracionButton;
        private System.Windows.Forms.ProgressBar ejecutarProgressBar;
        private System.Windows.Forms.Button corregirButton;
        private System.Windows.Forms.Button exportarButton;
        private System.Windows.Forms.SaveFileDialog exportarTextoSaveFileDialog;
        private System.ComponentModel.BackgroundWorker ejecutarBackgroundWorker;
        private System.ComponentModel.BackgroundWorker corregirBackgroundWorker;
    }
}