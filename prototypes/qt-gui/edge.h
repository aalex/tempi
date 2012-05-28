/****************************************************************************
 **
 ** Copyright (C) 2006-2006 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

 #ifndef EDGE_H
 #define EDGE_H

 #include <QGraphicsItem>

 class Node;

 class Edge : public QGraphicsItem
 {
 public:
     Edge(Node *sourceNode, Node *destNode);
     ~Edge();

     Node *sourceNode() const;
     void setSourceNode(Node *node);

     Node *destNode() const;
     void setDestNode(Node *node);

     void adjust();

     enum { Type = UserType + 2 };
     int type() const { return Type; }

 protected:
     QRectF boundingRect() const;
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 private:
     Node *source, *dest;

     QPointF sourcePoint;
     QPointF destPoint;
     qreal arrowSize;
 };

 #endif
