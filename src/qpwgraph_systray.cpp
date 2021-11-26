// qpwgraph_systray.cpp
//
/****************************************************************************
   Copyright (C) 2021, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include "qpwgraph_systray.h"


#ifdef CONFIG_SYSTEM_TRAY

#include <QWidget>
#include <QAction>

#include <QApplication>


//----------------------------------------------------------------------------
// qpwgraph_systray -- Custom system tray icon.

// Constructor.
qpwgraph_systray::qpwgraph_systray ( QWidget *widget )
	: QSystemTrayIcon(widget), m_widget(widget)
{
	// Set things as inherited...
	QSystemTrayIcon::setIcon(m_widget->windowIcon());
	QSystemTrayIcon::setToolTip(m_widget->windowTitle());

	m_show = m_menu.addAction(tr("Show/Hide"), this, SLOT(showHide()));
	m_quit = m_menu.addAction(tr("Quit"), this, SLOT(closeQuit()));

	QSystemTrayIcon::setContextMenu(&m_menu);

	QObject::connect(this,
		SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		SLOT(activated(QSystemTrayIcon::ActivationReason)));

	QSystemTrayIcon::show();
}


// Update context menu.
void qpwgraph_systray::updateContextMenu (void)
{
	if (m_widget->isVisible() && !m_widget->isMinimized())
		m_show->setText(tr("Hide"));
	else
		m_show->setText(tr("Show"));
}


// Handle systeam tray activity.
void qpwgraph_systray::activated ( QSystemTrayIcon::ActivationReason reason )
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		showHide();
		// Fall trhu...
	case QSystemTrayIcon::MiddleClick:
	case QSystemTrayIcon::DoubleClick:
	case QSystemTrayIcon::Unknown:
	default:
		break;
	}
}


// Handle menu actions.
void qpwgraph_systray::showHide (void)
{
	if (m_widget->isVisible() && !m_widget->isMinimized()) {
		// Hide away from sight, totally...
		m_widget->hide();
	} else {
		// Show normally.
		m_widget->show();
		m_widget->raise();
		m_widget->activateWindow();
	}
}


void qpwgraph_systray::closeQuit (void)
{
	m_widget->hide();

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QApplication::exit(0);
#else
	QApplication::quit();
#endif
}


#endif	// CONFIG_SYSTEM_TRAY

// end of qpwgraph_systray.cpp
