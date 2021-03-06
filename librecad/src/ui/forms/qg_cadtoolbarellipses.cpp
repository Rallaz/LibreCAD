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
#include "qg_cadtoolbarellipses.h"

#include "qg_cadtoolbar.h"

/*
 *  Constructs a QG_CadToolBarEllipses as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_CadToolBarEllipses::QG_CadToolBarEllipses(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
{
    setupUi(this);
    parentTB=static_cast<QG_CadToolBar*>(parent);
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_CadToolBarEllipses::~QG_CadToolBarEllipses()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_CadToolBarEllipses::languageChange()
{
    retranslateUi(this);
}

void QG_CadToolBarEllipses::init() {
    actionHandler = NULL;
    cadToolBar = NULL;
}

void QG_CadToolBarEllipses::mousePressEvent(QMouseEvent* e) {
    if (e->button()==Qt::RightButton && cadToolBar!=NULL) {
        cadToolBar->back();
        e->accept();
    }
}

void QG_CadToolBarEllipses::contextMenuEvent(QContextMenuEvent *e) {
    e->accept();
}

void QG_CadToolBarEllipses::setCadToolBar(QG_CadToolBar* tb) {
    cadToolBar = tb;
    if (tb!=NULL) {
        actionHandler = tb->getActionHandler();
    } else {
        RS_DEBUG->print(RS_Debug::D_ERROR,
                        "QG_CadToolBarEllipses::setCadToolBar(): No valid toolbar set.");
    }
}

void QG_CadToolBarEllipses::drawEllipseAxis() {
    if (cadToolBar!=NULL && actionHandler!=NULL) {
        actionHandler->slotDrawEllipseAxis();
    }
}

void QG_CadToolBarEllipses::drawEllipseArcAxis() {
    if (cadToolBar!=NULL && actionHandler!=NULL) {
        actionHandler->slotDrawEllipseArcAxis();
    }
}
void QG_CadToolBarEllipses::drawEllipseFociPoint() {
    if (cadToolBar!=NULL && actionHandler!=NULL) {
        actionHandler->slotDrawEllipseFociPoint();
    }
}
void QG_CadToolBarEllipses::drawEllipse4Points() {
    if (actionHandler!=NULL) {
        actionHandler->slotDrawEllipse4Points();
    }
}
void QG_CadToolBarEllipses::drawEllipseCenter3Points() {
    if (actionHandler!=NULL) {
        actionHandler->slotDrawEllipseCenter3Points();
    }
}
void QG_CadToolBarEllipses::drawEllipseInscribe() {
    if (actionHandler!=NULL) {
        actionHandler->slotDrawEllipseInscribe();
    }
}

void QG_CadToolBarEllipses::back() {
    if (cadToolBar!=NULL) {
        cadToolBar->showPreviousToolBar();
    }
}
//restore action from checked button
void QG_CadToolBarEllipses::restoreAction()
{
    if(actionHandler==NULL) return;
    if ( bEllipseAxes ->isChecked() ) {
        actionHandler->slotDrawEllipseAxis();
        return;
    }
    if ( bEllipseArcAxes ->isChecked() ) {
        actionHandler->slotDrawEllipseArcAxis();
        return;
    }
    if ( bEllipseFociPoint ->isChecked() ) {
        actionHandler->slotDrawEllipseFociPoint();
        return;
    }
    if ( bEllipse4Points ->isChecked() ) {
        actionHandler->slotDrawEllipse4Points();
        return;
    }
    if ( bEllipseCenter3Points ->isChecked() ) {
        actionHandler->slotDrawEllipseCenter3Points();
        return;
    }
    if ( bEllipseInscribe ->isChecked() ) {
        actionHandler->slotDrawEllipseInscribe();
        return;
    }
    //clear all action
    bHidden->setChecked(true);
    RS_ActionInterface* currentAction =actionHandler->getCurrentAction();
    if(currentAction != NULL) {
        currentAction->finish(false); //finish the action, but do not update toolBar
    }
}

void QG_CadToolBarEllipses::resetToolBar() {
    bHidden->setChecked(true);
}
void QG_CadToolBarEllipses::on_bBack_clicked()
{
    parentTB->showPreviousToolBar();
}


void QG_CadToolBarEllipses::showCadToolBar(RS2::ActionType actionType) {
    switch(actionType){
    case RS2::ActionDrawEllipseAxis:
        bEllipseAxes ->setChecked(true);
        return;
    case RS2::ActionDrawEllipseArcAxis:
        bEllipseArcAxes ->setChecked(true);
        return;
    case RS2::ActionDrawEllipseFociPoint:
        bEllipseFociPoint ->setChecked(true);
        return;
    case RS2::ActionDrawEllipse4Points:
        bEllipse4Points ->setChecked(true);
        return;
    case RS2::ActionDrawEllipseCenter3Points:
        bEllipseCenter3Points ->setChecked(true);
        return;
    case RS2::ActionDrawEllipseInscribe:
        bEllipseInscribe ->setChecked(true);
        return;
        default:
        bHidden->setChecked(true);
        return;
    }
}
