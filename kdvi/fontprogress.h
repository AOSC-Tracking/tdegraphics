// -*- C++ -*-
//
// fontprogress.h
//
// (C) 2001-2004 Stefan Kebekus
// Distributed under the GPL

#ifndef FONT_GENERATION_H
#define FONT_GENERATION_H

#include <kdialogbase.h>
#include <tqguardedptr.h>

class KProcIO;
class KProgress;
class TQLabel;


/**
 *  A dialog to give feedback to the user when kpsewhich is generating fonts.
 *
 * This class implements a dialog which pops up, shows a progress bar
 * and displays the MetaFont output. It contains three slots,
 * outputReceiver, setTotalSteps and hideDialog which can be connected
 * with the appropriate signals emitted by the fontpool class.
 *
 * @author Stefan Kebekus   <kebekus@kde.org>
 *
 *
 **/
class fontProgressDialog : public KDialogBase
{ 
    Q_OBJECT
  TQ_OBJECT

public:
    fontProgressDialog( const TQString& helpIndex, const TQString& label, const TQString& abortTip, const TQString& whatsThis, const TQString& ttip,
			TQWidget* parent = 0, const TQString &name = 0, bool progressbar=true );
    ~fontProgressDialog();
    
    /** The number of steps already done is increased, the text received
	here is analyzed and presented to the user. */
    void increaseNumSteps(const TQString& explanation);
    
    /** Used to initialize the progress bar. If the argument proc is
	non-zero, the associated process will be killed when the "abort"
	button is pressed. The FontProgress uses a TQGuarderPtr
	internally, so it is save to delete the KProcIO anytime. */
    void setTotalSteps(int, KProcIO *proc=0);
    
   TQLabel* TextLabel2;

private slots:
    /** Calling this slot does nothing than to kill the process that is
	pointed to be procIO, if procIO is not zero.*/
  void killProcIO();

private:
   TQLabel* TextLabel1;
   KProgress* ProgressBar1;
   int progress;
   TQGuardedPtr<KProcIO> procIO;
};

#endif // FONT_GENERATION_H
