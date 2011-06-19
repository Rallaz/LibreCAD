/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software 
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/

#include "rs_actiondrawlinebisector.h"

#include "rs_creation.h"
#include "rs_snapper.h"



RS_ActionDrawLineBisector::RS_ActionDrawLineBisector(
    RS_EntityContainer& container,
    RS_GraphicView& graphicView)
        :RS_PreviewActionInterface("Draw Bisectors", container, graphicView) {

    bisector = NULL;
    length = 10.0;
    line1 = NULL;
    line2 = NULL;
    number = 1;
    coord1 = RS_Vector(false);
    coord2 = RS_Vector(false);
    lastStatus = SetLine1;
}

QAction* RS_ActionDrawLineBisector::createGUIAction(RS2::ActionType /*type*/, QObject* /*parent*/) {
	// tr("&Bisector"),
    QAction* action = new QAction(tr("Bisector"), NULL);
	action->setIcon(QIcon(":/extui/linesbisector.png"));
    //action->zetStatusTip(tr("Draw bisectors"));
    return action;
}


void RS_ActionDrawLineBisector::trigger() {
    RS_PreviewActionInterface::trigger();

    //if (bisector!=NULL) {
    RS_Creation creation(container, graphicView);
    creation.createBisector(coord1,
                            coord2,
                            length,
                            number,
                            line1,
                            line2);
    /*RS_Entity* newEntity = NULL;

    newEntity = new RS_Line(container,
                            bisector->getData());

    if (newEntity!=NULL) {
        newEntity->setLayerToActive();
        newEntity->setPenToActive();
        container->addEntity(newEntity);

        // upd. undo list:
        if (document!=NULL) {
            document->startUndoCycle();
            document->addUndoable(newEntity);
            document->endUndoCycle();
        }
        graphicView->redraw(RS2::RedrawDrawing);
        setStatus(SetLine1);
}
    //reset();
    delete bisector;
    bisector = NULL;
    */
    /*} else {
        RS_DEBUG->print("RS_ActionDrawLineBisector::trigger:"
                        " Entity is NULL\n");
}*/
}



void RS_ActionDrawLineBisector::mouseMoveEvent(RS_MouseEvent* e) {
    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent begin");

    RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                graphicView->toGraphY(e->y()));

    switch (getStatus()) {
    case SetLine1:
        break;

    case SetLine2: {
            coord2 = mouse;
            RS_Entity* en = catchEntity(e, RS2::ResolveAll);
            if (en!=NULL && en->rtti()==RS2::EntityLine) {
                line2 = (RS_Line*)en;

                deletePreview();

                RS_Creation creation(preview, NULL, false);
                creation.createBisector(coord1,
                                        coord2,
                                        length,
                                        number,
                                        line1,
                                        line2);
                drawPreview();
            }
        }
        break;

    default:
        break;
    }

    RS_DEBUG->print("RS_ActionDrawLineBisector::mouseMoveEvent end");
}



void RS_ActionDrawLineBisector::mouseReleaseEvent(RS_MouseEvent* e) {

    if (e->button()==Qt::RightButton) {
        deletePreview();
        init(getStatus()-1);
    } else {

        RS_Vector mouse = RS_Vector(graphicView->toGraphX(e->x()),
                                    graphicView->toGraphY(e->y()));

        switch (getStatus()) {
        case SetLine1: {
                coord1 = mouse;
                RS_Entity* en = catchEntity(e, RS2::ResolveAll);
                if (en!=NULL && en->rtti()==RS2::EntityLine) {
                    line1 = (RS_Line*)en;
                }
            }
            setStatus(SetLine2);
            break;

        case SetLine2:
            coord2 = mouse;
            trigger();
            setStatus(SetLine1);
            break;
        }
    }

}


void RS_ActionDrawLineBisector::commandEvent(RS_CommandEvent* e) {
    RS_String c = e->getCommand().lower();

    if (checkCommand("help", c)) {
        RS_DIALOGFACTORY->commandMessage(msgAvailableCommands()
                                         + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        lastStatus = (Status)getStatus();
        if (checkCommand("length", c)) {
            deletePreview();
            setStatus(SetLength);
        } else if (checkCommand("number", c)) {
            deletePreview();
            setStatus(SetNumber);
        }
        break;

    case SetLength: {
            bool ok;
            double l = RS_Math::eval(c, &ok);
            if (ok==true) {
                length = l;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;

    case SetNumber: {
            bool ok;
            int n = (int)RS_Math::eval(c, &ok);
            if (ok==true) {
                number = n;
            } else {
                RS_DIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            RS_DIALOGFACTORY->requestOptions(this, true, true);
            setStatus(lastStatus);
        }
        break;


    default:
        break;
    }
}



RS_StringList RS_ActionDrawLineBisector::getAvailableCommands() {
    RS_StringList cmd;

    switch (getStatus()) {
    case SetLine1:
    case SetLine2:
        cmd += command("length");
        cmd += command("number");
        break;
    default:
        break;
    }

    return cmd;
}


void RS_ActionDrawLineBisector::updateMouseButtonHints() {
    switch (getStatus()) {
    case SetLine1:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select first line"),
                                            tr("Cancel"));
        break;
    case SetLine2:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Select second line"),
                                            tr("Back"));
        break;
    case SetLength:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter bisector length:"),
                                            tr("Back"));
        break;
    case SetNumber:
        RS_DIALOGFACTORY->updateMouseWidget(tr("Enter number of bisectors:"),
                                            tr("Back"));
        break;
    default:
        RS_DIALOGFACTORY->updateMouseWidget("", "");
        break;
    }
}



void RS_ActionDrawLineBisector::showOptions() {
    RS_ActionInterface::showOptions();

    RS_DIALOGFACTORY->requestOptions(this, true);
}



void RS_ActionDrawLineBisector::hideOptions() {
    RS_ActionInterface::hideOptions();

    RS_DIALOGFACTORY->requestOptions(this, false);
}



void RS_ActionDrawLineBisector::updateMouseCursor() {
    graphicView->setMouseCursor(RS2::CadCursor);
}



void RS_ActionDrawLineBisector::updateToolBar() {
    RS_DIALOGFACTORY->requestToolBar(RS2::ToolBarLines);
}



// EOF
