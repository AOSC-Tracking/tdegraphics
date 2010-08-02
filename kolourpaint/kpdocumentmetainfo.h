
/*
   Copyright (c) 2003,2004,2005 Clarence Dang <dang@kde.org>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef KP_DOCUMENT_META_INFO
#define KP_DOCUMENT_META_INFO


#include <tqimage.h>
#include <tqmap.h>
#include <tqstring.h>
#include <tqvaluelist.h>


class TQPoint;


class kpDocumentMetaInfo
{
public:
    kpDocumentMetaInfo ();
    kpDocumentMetaInfo (const kpDocumentMetaInfo &rhs);
    virtual ~kpDocumentMetaInfo ();

private:
    bool operator== (const kpDocumentMetaInfo &rhs) const;
    bool operator!= (const kpDocumentMetaInfo &rhs) const;

public:
    kpDocumentMetaInfo &operator= (const kpDocumentMetaInfo &rhs);


    void printDebug (const TQString &prefix) const;


    // See TQImage documentation

    int dotsPerMeterX () const;
    void setDotsPerMeterX (int val);

    int dotsPerMeterY () const;
    void setDotsPerMeterY (int val);


    TQPoint offset () const;
    void setOffset (const TQPoint &point);


    TQMap <TQImageTextKeyLang, TQString> textMap () const;
    TQValueList <TQImageTextKeyLang> textList () const;

    TQString text (const TQImageTextKeyLang &itkl) const;
    TQString text (const char *key, const char *lang) const;
    void setText (const TQImageTextKeyLang &itkl, const TQString &string);
    void setText (const char *key, const char *lang, const TQString &string);


private:
    // There is no need to maintain binary compatibility at this stage.
    // The d-pointer is just so that you can experiment without recompiling
    // the kitchen sink.
    class kpDocumentMetaInfoPrivate *d;
};


#endif  // KP_DOCUMENT_META_INFO
