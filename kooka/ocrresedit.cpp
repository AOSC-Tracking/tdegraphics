/***************************************************************************
                   ocrresedit.cpp - ocr result editor widget
                             -------------------
    begin                : Tue 12 Feb 2003
    copyright            : (C) 2003 by Klaas Freitag
    email                : freitag@suse.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This file may be distributed and/or modified under the terms of the    *
 *  GNU General Public License version 2 as published by the Free Software *
 *  Foundation and appearing in the file COPYING included in the           *
 *  packaging of this file.                                                *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any version of the KADMOS ocr/icr engine of reRecognition GmbH,   *
 *  Kreuzlingen and distribute the resulting executable without            *
 *  including the source code for KADMOS in the source distribution.       *
 *
 *  As a special exception, permission is given to link this program       *
 *  with any edition of TQt, and distribute the resulting executable,       *
 *  without including the source code for TQt in the source distribution.   *
 *                                                                         *
 ***************************************************************************/
#include <tqcolor.h>

#include "ocrresedit.h"
#include "ocrword.h"
#include <kdebug.h>
#include <kfiledialog.h>
#include <klocale.h>

#include <tqfile.h>
#include <tqtextstream.h>

/* -------------------- ocrResEdit -------------------- */

ocrResEdit::ocrResEdit( TQWidget *tqparent )
    : TQTextEdit(tqparent)
{
    m_updateColor.setNamedColor( "SeaGreen");
    m_ignoreColor.setNamedColor( "CadetBlue4" );
    m_wrnColor.setNamedColor( "firebrick2" );
}


void ocrResEdit::slMarkWordWrong( int line, const ocrWord& word )
{
    // m_textEdit->setSelection( line,
    slReplaceWord( line, word, word, m_wrnColor );
}


void ocrResEdit::slUpdateOCRResult( int line, const TQString& wordFrom,
                                   const TQString& wordTo )
{
    /* the index is quite useless here, since  the text could have had been
     * changed by corrections before. Thus better search the word and update
     * it.
     */
    slReplaceWord( line, wordFrom, wordTo, m_updateColor );

}


void ocrResEdit::slIgnoreWrongWord( int line, const ocrWord& word )
{
    slReplaceWord( line, word, word, m_ignoreColor );
}


void ocrResEdit::slSelectWord( int line, const ocrWord& word )
{
   if( line < paragraphs() )
   {
      TQString editLine = text(line);
      int cnt = editLine.tqcontains( word);

      if( cnt > 0 )
      {
	 int pos = editLine.tqfind(word);
	 setCursorPosition( line, pos );
	 setSelection( line, pos, line, pos + word.length());
      }
   }
}

void ocrResEdit::slReplaceWord( int line, const TQString& wordFrom,
                               const TQString& wordTo, const TQColor& color )
{
    kdDebug(28000) << "Updating word " << wordFrom << " in line " << line << endl;

    bool isRO = isReadOnly();

    if( line < paragraphs() )
    {
        TQString editLine = text(line);
        int cnt = editLine.tqcontains( wordFrom );

        if( cnt > 0 )
        {
            int pos = editLine.tqfind(wordFrom);
            setSelection( line, pos, line, pos+wordFrom.length());

            TQColor saveCol = this->color();
            setColor( color );
            if( isRO ) {
                setReadOnly(false);
            }
            insert( wordTo, unsigned (4) );
            if( isRO ) {
                setReadOnly( true );
            }
            setColor(saveCol);
        }
        else
        {
            kdDebug(28000) << "WRN: Paragraph does not contain word " << wordFrom << endl;
        }

    }
    else
    {
        kdDebug(28000) << "WRN: editor does not have line " << line << endl;
    }
}


void ocrResEdit::slSaveText()
{
    TQString fileName = KFileDialog::getSaveFileName( (TQDir::home()).path(),
                                                 "*.txt",
                                                 this,
                                                 i18n("Save OCR Result Text") );
    if( fileName.isEmpty() )
      return;
    TQFile file( fileName );
    if ( file.open( IO_WriteOnly ) )
    {
        TQTextStream stream( &file );
        stream << text();
        file.close();
    }
}

#include "ocrresedit.moc"
/*   */
