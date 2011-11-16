/** 
 * Copyright (C) 1997-2003 the KGhostView authors. See file AUTHORS.
 * 	
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __KGV_DOCUMENT_H__
#define __KGV_DOCUMENT_H__

#include <tqprinter.h>
#include <tqsize.h>
#include <tqstring.h>

#include "kgv.h"
#include "dscparse_adapter.h"
#include "kgv_view.h"

class KPrinter;
class KTempFile;
class Pdf2dsc;

class KGVDocument : public TQObject
{
    Q_OBJECT
  TQ_OBJECT

public:
    enum Format { PS, PDF };

    KGVDocument( KGVPart* part, const char* name = 0 );
    ~KGVDocument();

    void readSettings();

    /**
     * Is a file currently open?
     */
    bool isOpen() const;

    /**
     * Open the @em local file @p filename asynchronously.
     */
    void openFile( const TQString& filename, const TQString& mimetype );

    /**
     * Close the document.
     */
    void close();

    const TQString& fileName() const { return _fileName; }
    FILE* psFile() { return _psFile; }

    Format format() const { return _format; }

    /**
     * @return the document structure for the current document, or 0 if no
     * file is loaded.
     */ 
    KDSC* const dsc() const;

    /**
     * A list of page media (sizes).
     */
    TQStringList mediaNames() const;

    const CDSCMEDIA* findMediaByName( const TQString& mediaName ) const;

    TQSize computePageSize( const TQString& pageMedia ) const;

    static TQString pageSizeToString( TQPrinter::PageSize );

    /**
     * Returns a TQString which contains a range representation of @p pageList.
     * Examples: [1,3]       -> "1,3"
     *           [1,2,3]     -> "1-3"
     *           [1,3,4,5,8] -> "1,3-5,8"
     */
    static TQString pageListToRange( const KGV::PageList& );

public slots:
    void fileChanged( const TQString& );

    void saveAs();
    void print();

signals:
    /**
     * Emitted if a fileChanged() call fails.
     */
    void fileChangeFailed();

    void completed();
    void canceled( const TQString& );
    
protected:
    void scanDSC();
    void clearTemporaryFiles();
    
    /** 
     * Tries to uncompress the file. Returns true if it uncompressed the file
     * ( it changes _fileName to point to the uncompressed version of the file ).
     * Else, it does nothing and returns false.
     *
     * What type of files this is able to uncompress will depend on the
     * tdelibs installed. Generally it will work for .gz and .bz2
     */
    bool uncompressFile();
    void openPSFile(const TQString &file=TQString());

protected:
    bool savePages( const TQString& saveFileName,
                    const KGV::PageList& pageList );
    
    bool psCopyDoc( const TQString& inputFile, const TQString& outputFile,
                    const KGV::PageList& pageList );

    bool convertFromPDF( const TQString& saveFileName, 
                         unsigned int firstPage, unsigned int lastPage );

protected slots:
    void doOpenFile();
    void openPDFFileContinue( bool pdf2dscResult );

private:
    FILE* _psFile;

    TQString _fileName;
    TQString _mimetype;

    KGVPart*   _part;

    Format _format;

    KTempFile* _tmpUnzipped;
    KTempFile* _tmpFromPDF;
    KTempFile* _tmpDSC;

    Pdf2dsc* _pdf2dsc;

    TQString _interpreterPath;

    bool _isFileOpen;

    KDSC* _dsc;
};


class Pdf2dsc : public TQObject
{
    Q_OBJECT
  TQ_OBJECT
	
public:
    Pdf2dsc( const TQString& ghostscriptPath, TQObject* parent = 0, const char* name = 0 );
    ~Pdf2dsc();
    
    void run( const TQString& pdfName, const TQString& dscName );
    void kill();
    
signals:
    void finished( bool result );

protected slots:
    void processExited();

private:
    KProcess* _process;
    TQString   _ghostscriptPath;
};


inline KDSC* const KGVDocument::dsc() const
{
    return _dsc;
}

inline bool KGVDocument::isOpen() const
{
    return _isFileOpen;
}


#endif

// vim:sw=4:sts=4:ts=8:sta:tw=78:noet
