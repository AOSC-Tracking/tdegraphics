#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

#include <tqdialog.h>
class TQComboBox;
class TQPushButton;


class ViewControl : public TQDialog
{
	TQ_OBJECT
  
public:
	ViewControl( TQWidget *parent, const char *name );
	TQComboBox* magComboBox;
	TQComboBox* mediaComboBox;
	TQComboBox* orientComboBox;
	TQPushButton *apply;

	/**
	 * Update the mag combo box.
	 **/
	void updateMag (int mag);

	/**
	 * Enable/disable the apply button.
	 **/
	void applyEnable (bool enable);

protected:
        int prevmag, prevmedia, prevorient;

public slots:
	void slotApplyClicked();
	void slotMagSelection (int i);
	void slotMediaSelection (int i);
	void slotOrientSelection (int i);

signals:
	void applyChanges();
};

#endif
