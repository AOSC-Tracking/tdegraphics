#ifndef history_h
#define history_h

// history.h
//
// (C) 2001 Stefan Kebekus
// Distributed under the GPL

#include <tqobject.h>
#include <tqvaluelist.h>

#define HISTORYLENGTH 10

class HistoryItem
{
 public:
  HistoryItem(TQ_UINT32, TQ_UINT32);
  HistoryItem() {}

  bool operator== (const HistoryItem& item) const;

  TQ_UINT32 page;
  TQ_UINT32 ypos;
};

inline
bool operator!=(const HistoryItem& lhs, const HistoryItem& rhs)
{
  return !(lhs == rhs);
}


class History : public TQObject
{
  TQ_OBJECT
  

public:
  History();

  void          add(TQ_UINT32 page, TQ_UINT32 ypos);
  void          clear();

  HistoryItem*  forward();
  HistoryItem*  back();

signals:
  void backItem(bool);
  void forwardItem(bool);

private:
  // List of history items. First item is the oldest.
  TQValueList<HistoryItem> historyList;

  TQValueList<HistoryItem>::iterator currentItem;
};

#endif
