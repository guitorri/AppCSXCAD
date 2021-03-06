#ifndef APPCSXCAD_H
#define APPCSXCAD_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include "QCSXCAD.h"

#define __APPNAME__ "AppCSXCAD"

class AppCSXCAD : public QCSXCAD
{
    Q_OBJECT

public:
    AppCSXCAD(QWidget *parent = 0);
    virtual ~AppCSXCAD();

    virtual bool ReadFile(QString filename);

public slots:
	void Save();
	virtual void clear();

protected:
	void parseCommandLineArguments(const QStringList &argList);
	void SaveSettings();
	void LoadSettings();
	QString m_filename;
};

#endif // APPCSXCAD_H
