#ifndef KVIEWPART_IFACE_H
#define KVIEWPART_IFACE_H

#include <tdeparts/part.h>

class TQStringList;


class KViewPart_Iface : public KParts::ReadOnlyPart
{
  TQ_OBJECT
  
public:
  KViewPart_Iface(TQObject *parent, const char *name)
    : KParts::ReadOnlyPart(parent, name) {}

  virtual ~KViewPart_Iface() {}

  /* Returns a description of the current page size, for use in the
     statusbar of the kviewshell that embeds this KViewPart. */
  virtual TQString pageSizeDescription() = 0;

  /* This method calls closeURL(), but asks first ("The document was
     modified. Do you really want to close?") if the document has been
     modified after it has been loaded.  */
  virtual bool closeURL_ask() = 0;

  virtual TQStringList supportedMimeTypes() = 0;

public slots:
  virtual void slotSetFullPage(bool fullpage) = 0;
  virtual TQStringList fileFormats() const = 0;
};

#endif
