

#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

#include "AppCSXCAD.h"

#include "QVTKStructure.h"
#include "QCSXCAD_Global.h"

AppCSXCAD::AppCSXCAD(QWidget *parent) : QCSXCAD(parent)
{
	QMenuBar* mb = menuBar();
	QMenu *FileMenu = mb->addMenu("&File");

	if (QCSX_Settings.GetEdit())
		FileMenu->addAction(QIcon(":/images/filenew.png"),tr("New"),this,SLOT(New()),QKeySequence(tr("Ctrl+N")));
	FileMenu->addAction(QIcon(":/images/fileopen.png"),tr("Load"),this,SLOT(ImportGeometry()),QKeySequence(tr("Ctrl+L")));
	FileMenu->addSeparator();
	if (QCSX_Settings.GetEdit())
	{
		FileMenu->addAction(QIcon(":/images/filesave.png"),tr("Save"),this,SLOT(Save()),QKeySequence(tr("Ctrl+S")));
		FileMenu->addAction(QIcon(":/images/filesave.png"),tr("Save As"),this,SLOT(ExportGeometry()));
	}
	//FileMenu->addAction(QIcon(":/images/filesaveas.png"),tr("Save As.."),this,SLOT(SaveAs()));
	QMenu *FileExportMenu = FileMenu->addMenu(tr("E&xport"));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("CSXCAD XML"),this,SLOT(ExportGeometry()));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("Pov&ray"),this,SLOT(ExportGeometry_Povray()),QKeySequence(tr("Ctrl+R")));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("Polydata-&VTK"),this,SLOT(ExportGeometry_PolyDataVTK()));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("STL"),this,SLOT(ExportGeometry_STL()));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("PLY"),this,SLOT(ExportGeometry_PLY()));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("3D view to &PNG"),this,SLOT(ExportView2Image()),QKeySequence(tr("Ctrl+P")));
	FileExportMenu->addAction(QIcon(":/images/filesave.png"),tr("&X3D"),this,SLOT(ExportGeometry_X3D()),QKeySequence(tr("Ctrl+3")));
	FileMenu->addSeparator();
	FileMenu->addAction(QIcon(":/images/exit.png"),tr("Exit"),qApp,SLOT(quit()));

	QMenu *InfoMenu = mb->addMenu("Info");
	InfoMenu->addAction(tr("About"),this,SLOT(aboutQCSXCAD()));

	QStringList argList=qApp->arguments();
	if (argList.size()>1)
	{
		QString file = argList.back();
		if (!file.startsWith("-"))
			if (ReadFile(file)==false)
				QMessageBox::information(this,"File Error!",tr("Can't open file: %1").arg(file));
		GUIUpdate();
	}

	QString title = tr("AppCSXCAD");
	if (QCSX_Settings.GetEdit()==false)
		title += " - " + tr("View Mode");
	setWindowTitle(title);
	View3D();
	LoadSettings();

	parseCommandLineArguments(qApp->arguments());
}

AppCSXCAD::~AppCSXCAD()
{
	SaveSettings();
}

void AppCSXCAD::parseCommandLineArguments(const QStringList &argList)
{
	for (int n=0;n<argList.size();++n)
	{
		QString arg = argList.at(n);
		if (!arg.startsWith("--"))
			continue;
		QStringList arg_split = arg.split("=",QString::SkipEmptyParts);

		if (arg_split.size()==0)
			continue;

		//compate with all known args
		if (arg_split.at(0).compare("--export-polydata-vtk")==0)
		{
			cout << "AppCSXCAD - exporting 3D view to polydata vtk" << endl;
			if (arg_split.size()<=1)
				continue;
			ExportGeometry_PolyDataVTK(arg_split.at(1));
		}
		if (arg_split.at(0).compare("--export-STL")==0)
		{
			cout << "AppCSXCAD - exporting 3D data to STL" << endl;
			if (arg_split.size()<=1)
				continue;
			ExportGeometry_STL(arg_split.at(1));
		}
	}
}

void AppCSXCAD::SaveSettings()
{
	QSettings settings(__APPNAME__,__APPNAME__);
	settings.beginGroup("MainWindow");
	settings.setValue("Geometry",saveGeometry());
	settings.setValue("State",saveState());
	settings.endGroup();
}

void AppCSXCAD::LoadSettings()
{
	QSettings settings(__APPNAME__, __APPNAME__);

	settings.beginGroup("MainWindow");
	restoreGeometry(settings.value("Geometry").toByteArray());
	restoreState(settings.value("State").toByteArray());
	settings.endGroup();
}


bool AppCSXCAD::ReadFile(QString filename)
{
	m_filename=filename;
	return QCSXCAD::ReadFile(filename);
}

void AppCSXCAD::Save()
{
	Write2XML(m_filename);
}

void AppCSXCAD::clear()
{
	m_filename=QString();
	QCSXCAD::clear();
}

