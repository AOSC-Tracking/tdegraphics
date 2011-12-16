/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** TQt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/


void KSnapshotWidget::slotModeChanged( int mode )
{
    switch ( mode )
    {
    case 0:
	cbIncludeDecorations->setEnabled(false);
	break;
    case 1:
	cbIncludeDecorations->setEnabled(true);
	break;
    case 2:
	cbIncludeDecorations->setEnabled(false);
	break;
    case 3:
	cbIncludeDecorations->setEnabled(false);
	break;
    default:
	break;
    }

    spinDelay->setEnabled(mode != 2);
}


void KSnapshotWidget::setPreview( const TQPixmap &pm )
{
    TQImage img = pm.convertToImage();
    double r1 = ( ( double ) pm.height() ) / pm.width();
    if ( r1 * previewWidth()  < previewHeight() )
        img = img.smoothScale(  previewWidth(),
				int( previewWidth() * r1 ),
				TQ_ScaleMin );
    else
        img = img.smoothScale( ( int ) ( ( ( double )previewHeight() ) / r1 ),
			       previewHeight(), TQ_ScaleMin );

    TQToolTip::remove( lblImage );
    TQToolTip::add( lblImage,
        TQString( "Preview of the snapshot image (%1 x %2)" )
        .tqarg( pm.width() ).tqarg( pm.height() ) );

    lblImage->setPixmap( img );
    lblImage->adjustSize();
}


void KSnapshotWidget::setDelay( int i )
{
    spinDelay->setValue(i);
}


void KSnapshotWidget::setIncludeDecorations( bool b )
{
    cbIncludeDecorations->setChecked(b);
}


void KSnapshotWidget::setMode( int mode )
{
    comboMode->setCurrentItem(mode);
    slotModeChanged(mode);
}


int KSnapshotWidget::delay()
{
    return spinDelay->value();
}


bool KSnapshotWidget::includeDecorations()
{
    return cbIncludeDecorations->isChecked();
}


int KSnapshotWidget::mode()
{
    return comboMode->currentItem();
}


void KSnapshotWidget::slotNewClicked()
{
    emit newClicked();
}


void KSnapshotWidget::slotSaveClicked()
{
    emit saveClicked();
}


void KSnapshotWidget::slotPrintClicked()
{
    emit printClicked();
}


void KSnapshotWidget::slotStartDrag()
{
    emit startImageDrag();
}


TQPixmap KSnapshotWidget::preview()
{
    return *lblImage->pixmap();
}


int KSnapshotWidget::previewWidth()
{
    return lblImage->width();
}


int KSnapshotWidget::previewHeight()
{
    return lblImage->height();
}

void KSnapshotWidget::slotCopyClicked()
{
    emit copyClicked();
}
