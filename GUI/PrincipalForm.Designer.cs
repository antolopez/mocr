namespace OCR
{
    partial class PrincipalForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(PrincipalForm));
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.archivoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.abrirToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.guardarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.adquirirToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.twainToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.configuracionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.perfilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.preprocesadoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.manualPreprocesadoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.escaladoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.umbralizarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.enderezarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.automaticoPreprocesadoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.preferenciasPreprocesadoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.segmentaciónToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.manualSegmentacionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lineasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.correcionSlopeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.correcionSlantToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.palabrasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.caracteresToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.esqueletoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.semiautomaticoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.automaticoSegmentacionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.preferenciasSegmentacionToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reconocimientoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ManualToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.preferenciasReconocimientoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ayudaToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.acercaDeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.abrirFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.perfilesComboBox = new System.Windows.Forms.ComboBox();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.abrirImagenToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.twainToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.guardarImagenToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.perfilesToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.escalarToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.umbralizarToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.enderezarToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.lineasToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.correcionSlopeToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.correcionSlantToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.palabrasToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.caracteresToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.esqueletoToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.reconocimientoToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.guardarFileDialog = new System.Windows.Forms.SaveFileDialog();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.barraEstadoProgressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.estadoToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.tareaToolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.esqueletoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.preprocesadoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.segmentadoSemiautomaticoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.perfilesLabel = new OCR.LabelTrasparente();
            this.segmentadoAutomaticoBackgroundWorker = new System.ComponentModel.BackgroundWorker();
            this.menuStrip.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.archivoToolStripMenuItem,
            this.editarToolStripMenuItem,
            this.preprocesadoToolStripMenuItem,
            this.segmentaciónToolStripMenuItem,
            this.reconocimientoToolStripMenuItem,
            this.ayudaToolStripMenuItem});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(842, 24);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "menuStrip";
            // 
            // archivoToolStripMenuItem
            // 
            this.archivoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.abrirToolStripMenuItem,
            this.guardarToolStripMenuItem,
            this.adquirirToolStripMenuItem,
            this.configuracionToolStripMenuItem});
            this.archivoToolStripMenuItem.Name = "archivoToolStripMenuItem";
            this.archivoToolStripMenuItem.Size = new System.Drawing.Size(55, 20);
            this.archivoToolStripMenuItem.Text = "Archivo";
            // 
            // abrirToolStripMenuItem
            // 
            this.abrirToolStripMenuItem.Image = global::OCR.Properties.Resources.AbrirImagen;
            this.abrirToolStripMenuItem.Name = "abrirToolStripMenuItem";
            this.abrirToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.abrirToolStripMenuItem.Text = "Abrir";
            this.abrirToolStripMenuItem.Click += new System.EventHandler(this.abrirToolStripMenuItem_Click);
            // 
            // guardarToolStripMenuItem
            // 
            this.guardarToolStripMenuItem.Enabled = false;
            this.guardarToolStripMenuItem.Image = global::OCR.Properties.Resources.GuardarImagen;
            this.guardarToolStripMenuItem.Name = "guardarToolStripMenuItem";
            this.guardarToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.guardarToolStripMenuItem.Text = "Guardar";
            this.guardarToolStripMenuItem.Click += new System.EventHandler(this.guardarToolStripMenuItem_Click);
            // 
            // adquirirToolStripMenuItem
            // 
            this.adquirirToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.twainToolStripMenuItem});
            this.adquirirToolStripMenuItem.Name = "adquirirToolStripMenuItem";
            this.adquirirToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.adquirirToolStripMenuItem.Text = "Adquirir";
            // 
            // twainToolStripMenuItem
            // 
            this.twainToolStripMenuItem.Image = global::OCR.Properties.Resources.Escaner;
            this.twainToolStripMenuItem.Name = "twainToolStripMenuItem";
            this.twainToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.twainToolStripMenuItem.Text = "Twain";
            this.twainToolStripMenuItem.Click += new System.EventHandler(this.twainToolStripMenuItem_Click);
            // 
            // configuracionToolStripMenuItem
            // 
            this.configuracionToolStripMenuItem.Image = global::OCR.Properties.Resources.Preferencias;
            this.configuracionToolStripMenuItem.Name = "configuracionToolStripMenuItem";
            this.configuracionToolStripMenuItem.Size = new System.Drawing.Size(151, 22);
            this.configuracionToolStripMenuItem.Text = "Configuración";
            this.configuracionToolStripMenuItem.Click += new System.EventHandler(this.configuracionToolStripMenuItem_Click);
            // 
            // editarToolStripMenuItem
            // 
            this.editarToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.perfilesToolStripMenuItem});
            this.editarToolStripMenuItem.Name = "editarToolStripMenuItem";
            this.editarToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
            this.editarToolStripMenuItem.Text = "Editar";
            // 
            // perfilesToolStripMenuItem
            // 
            this.perfilesToolStripMenuItem.Image = global::OCR.Properties.Resources.Perfiles;
            this.perfilesToolStripMenuItem.Name = "perfilesToolStripMenuItem";
            this.perfilesToolStripMenuItem.Size = new System.Drawing.Size(120, 22);
            this.perfilesToolStripMenuItem.Text = "Perfiles";
            this.perfilesToolStripMenuItem.Click += new System.EventHandler(this.perfilesToolStripMenuItem_Click);
            // 
            // preprocesadoToolStripMenuItem
            // 
            this.preprocesadoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.manualPreprocesadoToolStripMenuItem,
            this.automaticoPreprocesadoToolStripMenuItem,
            this.preferenciasPreprocesadoToolStripMenuItem});
            this.preprocesadoToolStripMenuItem.Enabled = false;
            this.preprocesadoToolStripMenuItem.Name = "preprocesadoToolStripMenuItem";
            this.preprocesadoToolStripMenuItem.Size = new System.Drawing.Size(85, 20);
            this.preprocesadoToolStripMenuItem.Text = "Preprocesado";
            // 
            // manualPreprocesadoToolStripMenuItem
            // 
            this.manualPreprocesadoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.escaladoToolStripMenuItem,
            this.umbralizarToolStripMenuItem,
            this.enderezarToolStripMenuItem});
            this.manualPreprocesadoToolStripMenuItem.Enabled = false;
            this.manualPreprocesadoToolStripMenuItem.Name = "manualPreprocesadoToolStripMenuItem";
            this.manualPreprocesadoToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.manualPreprocesadoToolStripMenuItem.Text = "Manual";
            // 
            // escaladoToolStripMenuItem
            // 
            this.escaladoToolStripMenuItem.Enabled = false;
            this.escaladoToolStripMenuItem.Image = global::OCR.Properties.Resources.Escalar;
            this.escaladoToolStripMenuItem.Name = "escaladoToolStripMenuItem";
            this.escaladoToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.escaladoToolStripMenuItem.Text = "Escalar";
            this.escaladoToolStripMenuItem.Click += new System.EventHandler(this.escaladoToolStripMenuItem_Click);
            // 
            // umbralizarToolStripMenuItem
            // 
            this.umbralizarToolStripMenuItem.Enabled = false;
            this.umbralizarToolStripMenuItem.Image = global::OCR.Properties.Resources.Umbralizar;
            this.umbralizarToolStripMenuItem.Name = "umbralizarToolStripMenuItem";
            this.umbralizarToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.umbralizarToolStripMenuItem.Text = "Umbralizar";
            this.umbralizarToolStripMenuItem.Click += new System.EventHandler(this.umbralizarToolStripMenuItem_Click);
            // 
            // enderezarToolStripMenuItem
            // 
            this.enderezarToolStripMenuItem.Enabled = false;
            this.enderezarToolStripMenuItem.Image = global::OCR.Properties.Resources.Rotar;
            this.enderezarToolStripMenuItem.Name = "enderezarToolStripMenuItem";
            this.enderezarToolStripMenuItem.Size = new System.Drawing.Size(135, 22);
            this.enderezarToolStripMenuItem.Text = "Enderezar";
            this.enderezarToolStripMenuItem.Click += new System.EventHandler(this.enderezarToolStripMenuItem_Click);
            // 
            // automaticoPreprocesadoToolStripMenuItem
            // 
            this.automaticoPreprocesadoToolStripMenuItem.Enabled = false;
            this.automaticoPreprocesadoToolStripMenuItem.Name = "automaticoPreprocesadoToolStripMenuItem";
            this.automaticoPreprocesadoToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.automaticoPreprocesadoToolStripMenuItem.Text = "Automático";
            this.automaticoPreprocesadoToolStripMenuItem.Click += new System.EventHandler(this.automaticoPreprocesadoToolStripMenuItem_Click);
            // 
            // preferenciasPreprocesadoToolStripMenuItem
            // 
            this.preferenciasPreprocesadoToolStripMenuItem.Image = global::OCR.Properties.Resources.Preferencias;
            this.preferenciasPreprocesadoToolStripMenuItem.Name = "preferenciasPreprocesadoToolStripMenuItem";
            this.preferenciasPreprocesadoToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.preferenciasPreprocesadoToolStripMenuItem.Text = "Preferencias";
            this.preferenciasPreprocesadoToolStripMenuItem.Click += new System.EventHandler(this.preferenciasPreprocesadoToolStripMenuItem_Click);
            // 
            // segmentaciónToolStripMenuItem
            // 
            this.segmentaciónToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.manualSegmentacionToolStripMenuItem,
            this.semiautomaticoToolStripMenuItem,
            this.automaticoSegmentacionToolStripMenuItem,
            this.preferenciasSegmentacionToolStripMenuItem});
            this.segmentaciónToolStripMenuItem.Enabled = false;
            this.segmentaciónToolStripMenuItem.Name = "segmentaciónToolStripMenuItem";
            this.segmentaciónToolStripMenuItem.Size = new System.Drawing.Size(86, 20);
            this.segmentaciónToolStripMenuItem.Text = "Segmentación";
            // 
            // manualSegmentacionToolStripMenuItem
            // 
            this.manualSegmentacionToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lineasToolStripMenuItem,
            this.correcionSlopeToolStripMenuItem,
            this.correcionSlantToolStripMenuItem,
            this.palabrasToolStripMenuItem,
            this.caracteresToolStripMenuItem,
            this.esqueletoToolStripMenuItem});
            this.manualSegmentacionToolStripMenuItem.Enabled = false;
            this.manualSegmentacionToolStripMenuItem.Name = "manualSegmentacionToolStripMenuItem";
            this.manualSegmentacionToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.manualSegmentacionToolStripMenuItem.Text = "Manual";
            // 
            // lineasToolStripMenuItem
            // 
            this.lineasToolStripMenuItem.Image = global::OCR.Properties.Resources.Lineas;
            this.lineasToolStripMenuItem.Name = "lineasToolStripMenuItem";
            this.lineasToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.lineasToolStripMenuItem.Text = "Líneas";
            this.lineasToolStripMenuItem.Click += new System.EventHandler(this.líneasToolStripMenuItem_Click);
            // 
            // correcionSlopeToolStripMenuItem
            // 
            this.correcionSlopeToolStripMenuItem.Enabled = false;
            this.correcionSlopeToolStripMenuItem.Image = global::OCR.Properties.Resources.Slope;
            this.correcionSlopeToolStripMenuItem.Name = "correcionSlopeToolStripMenuItem";
            this.correcionSlopeToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.correcionSlopeToolStripMenuItem.Text = "Corrección Slope";
            this.correcionSlopeToolStripMenuItem.Click += new System.EventHandler(this.correcionSlopeToolStripMenuItem_Click);
            // 
            // correcionSlantToolStripMenuItem
            // 
            this.correcionSlantToolStripMenuItem.Enabled = false;
            this.correcionSlantToolStripMenuItem.Image = global::OCR.Properties.Resources.Slant;
            this.correcionSlantToolStripMenuItem.Name = "correcionSlantToolStripMenuItem";
            this.correcionSlantToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.correcionSlantToolStripMenuItem.Text = "Corrección Slant";
            this.correcionSlantToolStripMenuItem.Click += new System.EventHandler(this.correcionSlantToolStripMenuItem_Click);
            // 
            // palabrasToolStripMenuItem
            // 
            this.palabrasToolStripMenuItem.Enabled = false;
            this.palabrasToolStripMenuItem.Image = global::OCR.Properties.Resources.Palabras;
            this.palabrasToolStripMenuItem.Name = "palabrasToolStripMenuItem";
            this.palabrasToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.palabrasToolStripMenuItem.Text = "Palabras";
            this.palabrasToolStripMenuItem.Click += new System.EventHandler(this.palabrasToolStripMenuItem_Click);
            // 
            // caracteresToolStripMenuItem
            // 
            this.caracteresToolStripMenuItem.Enabled = false;
            this.caracteresToolStripMenuItem.Image = global::OCR.Properties.Resources.Caracteres;
            this.caracteresToolStripMenuItem.Name = "caracteresToolStripMenuItem";
            this.caracteresToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.caracteresToolStripMenuItem.Text = "Caracteres";
            this.caracteresToolStripMenuItem.Click += new System.EventHandler(this.caracteresToolStripMenuItem_Click);
            // 
            // esqueletoToolStripMenuItem
            // 
            this.esqueletoToolStripMenuItem.Enabled = false;
            this.esqueletoToolStripMenuItem.Image = global::OCR.Properties.Resources.Esqueleto;
            this.esqueletoToolStripMenuItem.Name = "esqueletoToolStripMenuItem";
            this.esqueletoToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.esqueletoToolStripMenuItem.Text = "Esqueleto";
            this.esqueletoToolStripMenuItem.Click += new System.EventHandler(this.esqueletoToolStripMenuItem_Click);
            // 
            // semiautomaticoToolStripMenuItem
            // 
            this.semiautomaticoToolStripMenuItem.Enabled = false;
            this.semiautomaticoToolStripMenuItem.Name = "semiautomaticoToolStripMenuItem";
            this.semiautomaticoToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.semiautomaticoToolStripMenuItem.Text = "Semiautomatico";
            this.semiautomaticoToolStripMenuItem.Click += new System.EventHandler(this.semiautomaticoToolStripMenuItem_Click);
            // 
            // automaticoSegmentacionToolStripMenuItem
            // 
            this.automaticoSegmentacionToolStripMenuItem.Enabled = false;
            this.automaticoSegmentacionToolStripMenuItem.Name = "automaticoSegmentacionToolStripMenuItem";
            this.automaticoSegmentacionToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.automaticoSegmentacionToolStripMenuItem.Text = "Automático";
            this.automaticoSegmentacionToolStripMenuItem.Click += new System.EventHandler(this.automaticoSegmentacionToolStripMenuItem_Click);
            // 
            // preferenciasSegmentacionToolStripMenuItem
            // 
            this.preferenciasSegmentacionToolStripMenuItem.Image = global::OCR.Properties.Resources.Preferencias;
            this.preferenciasSegmentacionToolStripMenuItem.Name = "preferenciasSegmentacionToolStripMenuItem";
            this.preferenciasSegmentacionToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.preferenciasSegmentacionToolStripMenuItem.Text = "Preferencias";
            this.preferenciasSegmentacionToolStripMenuItem.Click += new System.EventHandler(this.preferenciasSegmentacionToolStripMenuItem_Click);
            // 
            // reconocimientoToolStripMenuItem
            // 
            this.reconocimientoToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ManualToolStripMenuItem,
            this.preferenciasReconocimientoToolStripMenuItem});
            this.reconocimientoToolStripMenuItem.Enabled = false;
            this.reconocimientoToolStripMenuItem.Name = "reconocimientoToolStripMenuItem";
            this.reconocimientoToolStripMenuItem.Size = new System.Drawing.Size(94, 20);
            this.reconocimientoToolStripMenuItem.Text = "Reconocimiento";
            // 
            // ManualToolStripMenuItem
            // 
            this.ManualToolStripMenuItem.Enabled = false;
            this.ManualToolStripMenuItem.Image = global::OCR.Properties.Resources.Reconocimiento;
            this.ManualToolStripMenuItem.Name = "ManualToolStripMenuItem";
            this.ManualToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.ManualToolStripMenuItem.Text = "Manual";
            this.ManualToolStripMenuItem.Click += new System.EventHandler(this.ManualToolStripMenuItem_Click);
            // 
            // preferenciasReconocimientoToolStripMenuItem
            // 
            this.preferenciasReconocimientoToolStripMenuItem.Image = global::OCR.Properties.Resources.Preferencias;
            this.preferenciasReconocimientoToolStripMenuItem.Name = "preferenciasReconocimientoToolStripMenuItem";
            this.preferenciasReconocimientoToolStripMenuItem.Size = new System.Drawing.Size(145, 22);
            this.preferenciasReconocimientoToolStripMenuItem.Text = "Preferencias";
            this.preferenciasReconocimientoToolStripMenuItem.Click += new System.EventHandler(this.preferenciasReconocimientoToolStripMenuItem_Click);
            // 
            // ayudaToolStripMenuItem
            // 
            this.ayudaToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.acercaDeToolStripMenuItem});
            this.ayudaToolStripMenuItem.Name = "ayudaToolStripMenuItem";
            this.ayudaToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.ayudaToolStripMenuItem.Text = "Ayuda";
            // 
            // acercaDeToolStripMenuItem
            // 
            this.acercaDeToolStripMenuItem.Image = global::OCR.Properties.Resources.Acercade;
            this.acercaDeToolStripMenuItem.Name = "acercaDeToolStripMenuItem";
            this.acercaDeToolStripMenuItem.Size = new System.Drawing.Size(133, 22);
            this.acercaDeToolStripMenuItem.Text = "Acerca de";
            this.acercaDeToolStripMenuItem.Click += new System.EventHandler(this.acercaDeToolStripMenuItem_Click);
            // 
            // abrirFileDialog
            // 
            this.abrirFileDialog.Filter = "Archivos BMP (.bmp)|*.bmp";
            this.abrirFileDialog.Title = "Cargar imagen";
            this.abrirFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.abrirFileDialog_FileOk);
            // 
            // perfilesComboBox
            // 
            this.perfilesComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.perfilesComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.perfilesComboBox.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.perfilesComboBox.Location = new System.Drawing.Point(627, 3);
            this.perfilesComboBox.Name = "perfilesComboBox";
            this.perfilesComboBox.Size = new System.Drawing.Size(148, 21);
            this.perfilesComboBox.TabIndex = 2;
            this.perfilesComboBox.SelectedIndexChanged += new System.EventHandler(this.perfilesComboBox_SelectedIndexChanged);
            // 
            // toolStrip
            // 
            this.toolStrip.ImageScalingSize = new System.Drawing.Size(32, 32);
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.abrirImagenToolStripButton,
            this.twainToolStripButton,
            this.guardarImagenToolStripButton,
            this.toolStripSeparator1,
            this.perfilesToolStripButton,
            this.toolStripSeparator2,
            this.escalarToolStripButton,
            this.umbralizarToolStripButton,
            this.enderezarToolStripButton,
            this.toolStripSeparator3,
            this.lineasToolStripButton,
            this.correcionSlopeToolStripButton,
            this.correcionSlantToolStripButton,
            this.palabrasToolStripButton,
            this.caracteresToolStripButton,
            this.esqueletoToolStripButton,
            this.toolStripSeparator4,
            this.reconocimientoToolStripButton});
            this.toolStrip.Location = new System.Drawing.Point(0, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(842, 39);
            this.toolStrip.TabIndex = 5;
            this.toolStrip.Text = "toolStrip";
            // 
            // abrirImagenToolStripButton
            // 
            this.abrirImagenToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.abrirImagenToolStripButton.Image = global::OCR.Properties.Resources.AbrirImagen;
            this.abrirImagenToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.abrirImagenToolStripButton.Name = "abrirImagenToolStripButton";
            this.abrirImagenToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.abrirImagenToolStripButton.Text = "Abrir Imagen";
            this.abrirImagenToolStripButton.Click += new System.EventHandler(this.abrirToolStripMenuItem_Click);
            // 
            // twainToolStripButton
            // 
            this.twainToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.twainToolStripButton.Image = global::OCR.Properties.Resources.Escaner;
            this.twainToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.twainToolStripButton.Name = "twainToolStripButton";
            this.twainToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.twainToolStripButton.Text = "Escanear Imagen";
            this.twainToolStripButton.Click += new System.EventHandler(this.twainToolStripMenuItem_Click);
            // 
            // guardarImagenToolStripButton
            // 
            this.guardarImagenToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.guardarImagenToolStripButton.Enabled = false;
            this.guardarImagenToolStripButton.Image = global::OCR.Properties.Resources.GuardarImagen;
            this.guardarImagenToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.guardarImagenToolStripButton.Name = "guardarImagenToolStripButton";
            this.guardarImagenToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.guardarImagenToolStripButton.Text = "Guardar Imagen";
            this.guardarImagenToolStripButton.ToolTipText = "Guardar Imagen Preprocesada";
            this.guardarImagenToolStripButton.Click += new System.EventHandler(this.guardarToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 39);
            // 
            // perfilesToolStripButton
            // 
            this.perfilesToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.perfilesToolStripButton.Image = global::OCR.Properties.Resources.Perfiles;
            this.perfilesToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.perfilesToolStripButton.Name = "perfilesToolStripButton";
            this.perfilesToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.perfilesToolStripButton.Text = "Perfiles";
            this.perfilesToolStripButton.Click += new System.EventHandler(this.perfilesToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 39);
            // 
            // escalarToolStripButton
            // 
            this.escalarToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.escalarToolStripButton.Enabled = false;
            this.escalarToolStripButton.Image = global::OCR.Properties.Resources.Escalar;
            this.escalarToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.escalarToolStripButton.Name = "escalarToolStripButton";
            this.escalarToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.escalarToolStripButton.Text = "Escalar";
            this.escalarToolStripButton.Click += new System.EventHandler(this.escaladoToolStripMenuItem_Click);
            // 
            // umbralizarToolStripButton
            // 
            this.umbralizarToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.umbralizarToolStripButton.Enabled = false;
            this.umbralizarToolStripButton.Image = global::OCR.Properties.Resources.Umbralizar;
            this.umbralizarToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.umbralizarToolStripButton.Name = "umbralizarToolStripButton";
            this.umbralizarToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.umbralizarToolStripButton.Text = "Umbralizar";
            this.umbralizarToolStripButton.Click += new System.EventHandler(this.umbralizarToolStripMenuItem_Click);
            // 
            // enderezarToolStripButton
            // 
            this.enderezarToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.enderezarToolStripButton.Enabled = false;
            this.enderezarToolStripButton.Image = global::OCR.Properties.Resources.Rotar;
            this.enderezarToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.enderezarToolStripButton.Name = "enderezarToolStripButton";
            this.enderezarToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.enderezarToolStripButton.Text = "Enderezar";
            this.enderezarToolStripButton.Click += new System.EventHandler(this.enderezarToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 39);
            // 
            // lineasToolStripButton
            // 
            this.lineasToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.lineasToolStripButton.Enabled = false;
            this.lineasToolStripButton.Image = global::OCR.Properties.Resources.Lineas;
            this.lineasToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.lineasToolStripButton.Name = "lineasToolStripButton";
            this.lineasToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.lineasToolStripButton.Text = "Segmentar Líneas";
            this.lineasToolStripButton.Click += new System.EventHandler(this.líneasToolStripMenuItem_Click);
            // 
            // correcionSlopeToolStripButton
            // 
            this.correcionSlopeToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.correcionSlopeToolStripButton.Enabled = false;
            this.correcionSlopeToolStripButton.Image = global::OCR.Properties.Resources.Slope;
            this.correcionSlopeToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.correcionSlopeToolStripButton.Name = "correcionSlopeToolStripButton";
            this.correcionSlopeToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.correcionSlopeToolStripButton.Text = "Corregir Slope";
            this.correcionSlopeToolStripButton.Click += new System.EventHandler(this.correcionSlopeToolStripMenuItem_Click);
            // 
            // correcionSlantToolStripButton
            // 
            this.correcionSlantToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.correcionSlantToolStripButton.Enabled = false;
            this.correcionSlantToolStripButton.Image = global::OCR.Properties.Resources.Slant;
            this.correcionSlantToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.correcionSlantToolStripButton.Name = "correcionSlantToolStripButton";
            this.correcionSlantToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.correcionSlantToolStripButton.Text = "Corregir Slant";
            this.correcionSlantToolStripButton.Click += new System.EventHandler(this.correcionSlantToolStripMenuItem_Click);
            // 
            // palabrasToolStripButton
            // 
            this.palabrasToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.palabrasToolStripButton.Enabled = false;
            this.palabrasToolStripButton.Image = global::OCR.Properties.Resources.Palabras;
            this.palabrasToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.palabrasToolStripButton.Name = "palabrasToolStripButton";
            this.palabrasToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.palabrasToolStripButton.Text = "Segmentación Palabras";
            this.palabrasToolStripButton.Click += new System.EventHandler(this.palabrasToolStripMenuItem_Click);
            // 
            // caracteresToolStripButton
            // 
            this.caracteresToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.caracteresToolStripButton.Enabled = false;
            this.caracteresToolStripButton.Image = global::OCR.Properties.Resources.Caracteres;
            this.caracteresToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.caracteresToolStripButton.Name = "caracteresToolStripButton";
            this.caracteresToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.caracteresToolStripButton.Text = "Segmentar Caracteres";
            this.caracteresToolStripButton.Click += new System.EventHandler(this.caracteresToolStripMenuItem_Click);
            // 
            // esqueletoToolStripButton
            // 
            this.esqueletoToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.esqueletoToolStripButton.Enabled = false;
            this.esqueletoToolStripButton.Image = global::OCR.Properties.Resources.Esqueleto;
            this.esqueletoToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.esqueletoToolStripButton.Name = "esqueletoToolStripButton";
            this.esqueletoToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.esqueletoToolStripButton.Text = "Esqueleto";
            this.esqueletoToolStripButton.Click += new System.EventHandler(this.esqueletoToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 39);
            // 
            // reconocimientoToolStripButton
            // 
            this.reconocimientoToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.reconocimientoToolStripButton.Enabled = false;
            this.reconocimientoToolStripButton.Image = global::OCR.Properties.Resources.Reconocimiento;
            this.reconocimientoToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.reconocimientoToolStripButton.Name = "reconocimientoToolStripButton";
            this.reconocimientoToolStripButton.Size = new System.Drawing.Size(36, 36);
            this.reconocimientoToolStripButton.Text = "Reconocimiento";
            this.reconocimientoToolStripButton.Click += new System.EventHandler(this.ManualToolStripMenuItem_Click);
            // 
            // guardarFileDialog
            // 
            this.guardarFileDialog.Filter = "Archivos BMP (.bmp)|*.bmp";
            this.guardarFileDialog.Title = "Guardar imagen";
            this.guardarFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.guardarFileDialog_FileOk);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.barraEstadoProgressBar,
            this.estadoToolStripStatusLabel,
            this.tareaToolStripStatusLabel});
            this.statusStrip.LayoutStyle = System.Windows.Forms.ToolStripLayoutStyle.HorizontalStackWithOverflow;
            this.statusStrip.Location = new System.Drawing.Point(0, 477);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(842, 22);
            this.statusStrip.TabIndex = 7;
            this.statusStrip.Text = "statusStrip";
            // 
            // barraEstadoProgressBar
            // 
            this.barraEstadoProgressBar.Name = "barraEstadoProgressBar";
            this.barraEstadoProgressBar.Size = new System.Drawing.Size(100, 16);
            this.barraEstadoProgressBar.Visible = false;
            // 
            // estadoToolStripStatusLabel
            // 
            this.estadoToolStripStatusLabel.Name = "estadoToolStripStatusLabel";
            this.estadoToolStripStatusLabel.Size = new System.Drawing.Size(29, 17);
            this.estadoToolStripStatusLabel.Text = "Listo";
            // 
            // tareaToolStripStatusLabel
            // 
            this.tareaToolStripStatusLabel.Name = "tareaToolStripStatusLabel";
            this.tareaToolStripStatusLabel.Size = new System.Drawing.Size(0, 17);
            // 
            // esqueletoBackgroundWorker
            // 
            this.esqueletoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.esqueletoBackgroundWorker_DoWork);
            this.esqueletoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.esqueletoBackgroundWorker_RunWorkerCompleted);
            // 
            // preprocesadoBackgroundWorker
            // 
            this.preprocesadoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.preprocesadoBackgroundWorker_DoWork);
            this.preprocesadoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.preprocesadoBackgroundWorker_RunWorkerCompleted);
            // 
            // segmentadoSemiautomaticoBackgroundWorker
            // 
            this.segmentadoSemiautomaticoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.segmentadoSemiautomaticoBackgroundWorker_DoWork);
            this.segmentadoSemiautomaticoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.segmentadoSemiautomaticoBackgroundWorker_RunWorkerCompleted);
            // 
            // perfilesLabel
            // 
            this.perfilesLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.perfilesLabel.AutoSize = true;
            this.perfilesLabel.BackColor = System.Drawing.Color.Red;
            this.perfilesLabel.Location = new System.Drawing.Point(559, 6);
            this.perfilesLabel.Name = "perfilesLabel";
            this.perfilesLabel.Size = new System.Drawing.Size(65, 13);
            this.perfilesLabel.TabIndex = 3;
            // 
            // segmentadoAutomaticoBackgroundWorker
            // 
            this.segmentadoAutomaticoBackgroundWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.segmentadoAutomaticoBackgroundWorker_DoWork);
            this.segmentadoAutomaticoBackgroundWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.segmentadoAutomaticoBackgroundWorker_RunWorkerCompleted);
            // 
            // PrincipalForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(842, 499);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.toolStrip);
            this.Controls.Add(this.perfilesLabel);
            this.Controls.Add(this.perfilesComboBox);
            this.Controls.Add(this.menuStrip);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip;
            this.Name = "PrincipalForm";
            this.Text = "MOCR";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Principal_FormClosing);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem archivoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ayudaToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem adquirirToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem twainToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem abrirToolStripMenuItem;
        private System.Windows.Forms.OpenFileDialog abrirFileDialog;
        private System.Windows.Forms.ComboBox perfilesComboBox;
        private LabelTrasparente perfilesLabel;
        private System.Windows.Forms.ToolStripMenuItem preprocesadoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem manualPreprocesadoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem automaticoSegmentacionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem preferenciasPreprocesadoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem segmentaciónToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem manualSegmentacionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lineasToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem caracteresToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem semiautomaticoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem automaticoPreprocesadoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem preferenciasSegmentacionToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem reconocimientoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ManualToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem preferenciasReconocimientoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem perfilesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem configuracionToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripMenuItem acercaDeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem escaladoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem umbralizarToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton abrirImagenToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem enderezarToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem correcionSlopeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem correcionSlantToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem palabrasToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem guardarToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton guardarImagenToolStripButton;
        private System.Windows.Forms.SaveFileDialog guardarFileDialog;
        private System.Windows.Forms.ToolStripMenuItem esqueletoToolStripMenuItem;
        private System.Windows.Forms.ToolStripButton twainToolStripButton;
        private System.Windows.Forms.ToolStripButton lineasToolStripButton;
        private System.Windows.Forms.ToolStripButton perfilesToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripButton enderezarToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripButton umbralizarToolStripButton;
        private System.Windows.Forms.ToolStripButton escalarToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripButton reconocimientoToolStripButton;
        private System.Windows.Forms.ToolStripButton palabrasToolStripButton;
        private System.Windows.Forms.ToolStripButton caracteresToolStripButton;
        private System.Windows.Forms.ToolStripButton esqueletoToolStripButton;
        private System.Windows.Forms.ToolStripButton correcionSlopeToolStripButton;
        private System.Windows.Forms.ToolStripButton correcionSlantToolStripButton;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripProgressBar barraEstadoProgressBar;
        private System.Windows.Forms.ToolStripStatusLabel tareaToolStripStatusLabel;
        private System.Windows.Forms.ToolStripStatusLabel estadoToolStripStatusLabel;
        private System.ComponentModel.BackgroundWorker esqueletoBackgroundWorker;
        private System.ComponentModel.BackgroundWorker preprocesadoBackgroundWorker;
        private System.ComponentModel.BackgroundWorker segmentadoSemiautomaticoBackgroundWorker;
        private System.ComponentModel.BackgroundWorker segmentadoAutomaticoBackgroundWorker;
        
    }
}

