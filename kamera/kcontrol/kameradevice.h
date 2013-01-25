/*

    Copyright (C) 2001 The Kompany
		  2002-2003	Ilya Konstantinov <kde-devel@future.shiny.co.il>
		  2002-2003	Marcus Meissner <marcus@jet.franken.de>
		  2003		Nadeem Hasan <nhasan@nadmm.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

*/
#ifndef __kameradevice_h__
#define __kameradevice_h__

#include <tqmap.h>
#include <kdialogbase.h>
#include <config.h>

class TDEConfig;
class TQString;
class KListView;
class TQWidgetStack;
class TQVButtonGroup;
class TQVGroupBox;
class TQComboBox;
class TQLineEdit;
class TQRadioButton;

class KCamera : public TQObject {
	friend class KameraDeviceSelectDialog;
	Q_OBJECT
  
public:
	KCamera(const TQString &name, const TQString &path);
	~KCamera();
	void invalidateCamera();
	bool configure();
	void load(TDEConfig *m_config);
	void save(TDEConfig *m_config);
	bool test();
	TQStringList supportedPorts();

	Camera* camera();
	TQString name() const { return m_name ; }
	TQString model() const { return m_model; }
	TQString path() const { return m_path; }
	TQString portName();

	TQString summary();
	CameraAbilities abilities();

	void setName(const TQString &name);
	void setModel(const TQString &model);
	void setPath(const TQString &path);

	bool isTestable() const;
	bool isConfigurable();

signals:
	void error(const TQString &message);
	void error(const TQString &message, const TQString &details);
	
protected:
	bool initInformation();
	bool initCamera();
//	void doConfigureCamera(Camera *camera, CameraWidget *widgets);
//	int frontend_prompt(Camera *camera, CameraWidget *widgets);

	Camera *m_camera;
//	TDEConfig *m_config;
	TQString m_name; // the camera's real name
	TQString m_model;
	TQString m_path;
	CameraAbilities m_abilities;
	CameraAbilitiesList *m_abilitylist;
};

class KameraDeviceSelectDialog : public KDialogBase
{
	Q_OBJECT
  
public:
	KameraDeviceSelectDialog(TQWidget *parent, KCamera *device);
	void save();
	void load();
protected slots:
	void slot_setModel(TQListViewItem *item);
	void slot_error(const TQString &message);
	void slot_error(const TQString &message, const TQString &details);
protected:
	KCamera *m_device;
	
	bool populateCameraListView(void);
	void setPortType(int type);

	// port settings widgets
	KListView *m_modelSel;
	TQLineEdit *m_nameEdit;
	TQWidgetStack *m_settingsStack;
	TQVButtonGroup *m_portSelectGroup;
	TQVGroupBox *m_portSettingsGroup;
	TQComboBox *m_serialPortCombo;
	// port selection radio buttons
	TQRadioButton *m_serialRB;
	TQRadioButton *m_USBRB;
};

#endif
