// infodialog.cpp
//
// (C) 2001-2003 Stefan Kebekus
// Distributed under the GPL

#include <config.h>

#include <kdebug.h>
#include <tdeio/global.h>
#include <klocale.h>
#include <tqfile.h>
#include <tqlabel.h>
#include <tqlayout.h>
#include <tqregexp.h>
#include <tqtextview.h>
#include <tqtooltip.h>
#include <tqvariant.h>
#include <tqwhatsthis.h>

#include "dviFile.h"
#include "fontpool.h"
#include "infodialog.h"

infoDialog::infoDialog( TQWidget* parent )
  : KDialogBase( Tabbed, i18n("Document Info"), Ok, Ok, parent, "Document Info", false, false)
{
  TQFrame *page1 = addPage( i18n("DVI File") );
  TQVBoxLayout *topLayout1 = new TQVBoxLayout( page1, 0, 6 );
  TextLabel1 = new TQTextView( page1, "TextLabel1" );
  TQToolTip::add( TextLabel1, i18n("Information on the currently loaded DVI-file.") );
  topLayout1->addWidget( TextLabel1 );

  TQFrame *page2 = addPage( i18n("Fonts") );
  TQVBoxLayout *topLayout2 = new TQVBoxLayout( page2, 0, 6 );
  TextLabel2 = new TQTextView( page2, "TextLabel1" );
  TextLabel2->setMinimumWidth(fontMetrics().maxWidth()*40);
  TextLabel2->setMinimumHeight(fontMetrics().height()*10);
  TQToolTip::add( TextLabel2, i18n("Information on currently loaded fonts.") );
  TQWhatsThis::add( TextLabel2, i18n("This text field shows detailed information about the currently loaded fonts. "
				    "This is useful for experts who want to locate problems in the setup of TeX or KDVI.") );
  topLayout2->addWidget( TextLabel2 );

  TQFrame *page3 = addPage( i18n("External Programs") );
  TQVBoxLayout *topLayout3 = new TQVBoxLayout( page3, 0, 6 );
  TextLabel3 = new TQTextView( page3, "TextLabel1" );
  TextLabel3->setText( i18n("No output from any external program received.") );
  TQToolTip::add( TextLabel3, i18n("Output of external programs.") );
  TQWhatsThis::add( TextLabel3, i18n("KDVI uses external programs, such as MetaFont, dvipdfm or dvips. "
				    "This text field shows the output of these programs. "
				    "That is useful for experts who want to find problems in the setup of TeX or KDVI.") );
  topLayout3->addWidget( TextLabel3 );

  MFOutputReceived = false;
  headline         = TQString();
  pool             = TQString();
}


void infoDialog::setDVIData(dvifile *dviFile)
{
  TQString text = "";

  if (dviFile == NULL)
    text = i18n("There is no DVI file loaded at the moment.");
  else {
    text.append("<table WIDTH=\"100%\" NOSAVE >");
    text.append(TQString("<tr><td><b>%1</b></td> <td>%2</td></tr>").arg(i18n("Filename")).arg(dviFile->filename));

    TQFile file(dviFile->filename);
    if (file.exists())
      text.append(TQString("<tr><td><b>%1</b></td> <td>%2</td></tr>").arg(i18n("File Size")).arg(TDEIO::convertSize(file.size())));
    else
      text.append(TQString("<tr><td><b> </b></td> <td>%1</td></tr>").arg(i18n("The file does no longer exist.")));

    text.append(TQString("<tr><td><b>  </b></td> <td>  </td></tr>"));
    text.append(TQString("<tr><td><b>%1</b></td> <td>%2</td></tr>").arg(i18n("#Pages")).arg(dviFile->total_pages));
    text.append(TQString("<tr><td><b>%1</b></td> <td>%2</td></tr>").arg(i18n("Generator/Date")).arg(dviFile->generatorString));
  } // else (dviFile == NULL)

  TextLabel1->setText( text );
}


void infoDialog::setFontInfo(fontPool *fp)
{
  TextLabel2->setText(fp->status());
}

void infoDialog::outputReceiver(const TQString& _op)
{
  TQString op = _op;
  op = op.replace( TQRegExp("<"), "&lt;" );

  if (MFOutputReceived == false) {
    TextLabel3->setText("<b>"+headline+"</b><br>");
    headline = TQString();
  }

  // It seems that the TQTextView wants that we append only full lines.
  // We see to that.
  pool = pool+op;
  int idx = pool.findRev("\n");

  while(idx != -1) {
    TQString line = pool.left(idx);
    pool = pool.mid(idx+1);

    // If the Output of the kpsewhich program contains a line starting
    // with "kpathsea:", this means that a new MetaFont-run has been
    // started. We filter these lines out and print them in boldface.
    int startlineindex = line.find("kpathsea:");
    if (startlineindex != -1) {
      int endstartline  = line.find("\n",startlineindex);
      TQString startLine = line.mid(startlineindex,endstartline-startlineindex);
      if (MFOutputReceived)
	TextLabel3->append("<hr>\n<b>"+startLine+"</b>");
      else
	TextLabel3->append("<b>"+startLine+"</b>");
    TextLabel3->append(line.mid(endstartline));
    } else
      TextLabel3->append(line);
    idx = pool.findRev("\n");
  }

  MFOutputReceived = true;
}

void infoDialog::clear(const TQString& op)
{
  headline         = op;
  pool             = TQString();
  MFOutputReceived = false;
}
#include "infodialog.moc"
