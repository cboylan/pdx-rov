#include "mainwindow.h"
#include "createjoystickdialog.h"
#include "createvarmonitordialog.h"
#include "qjoystick.h"
#include "joysticksmodel.h"
#include "connectionmanager.h"
#include "servowidget.h"
#include "rovconnection.h"
#include "varmonitorbuilder.h"
#include "armjoystick.h"
#include "propjoystick.h"

#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_connManager(ConnectionManager::instance())
{
	m_joysticksModel = new JoysticksModel(this);
	m_joysticksTable = new QTableView(this);
	m_joysticksTable->setModel(m_joysticksModel);
	setCentralWidget(m_joysticksTable);
	setupActions();
	setupMenus();
	setupToolbars();
}

void MainWindow::addJoystick()
{
	CreateJoystickDialog *d = new CreateJoystickDialog(this);
	if(d->exec())
	{
		RovJoystick *j = 0;
		switch(d->type())
		{
			case RovJoystick::Arm:
				j = new ArmJoystick(d->path(), d->connection(), this);
				break;
			case RovJoystick::Propulsion:
				j = new PropJoystick(d->path(), d->connection(), this);
		}
		
		if(j)
		{
			m_joysticksModel->addJoystick(*j);
		}
	}
}

void MainWindow::addVarMonitor()
{
	CreateVarMonitorDialog *d = new CreateVarMonitorDialog(*m_connManager, this);
	if(d->exec())
	{
		VarMonitorBuilder::instance().createVarMonitor(d->connection(), d->name());
	}
}

void MainWindow::setupToolbars()
{
	m_toolBar = new QToolBar(this);
	m_toolBar->addAction(addConnectionAction);
	m_toolBar->addAction(addJoystickAction);
	m_toolBar->addAction(addVarMonitorAction);

	addToolBar(Qt::TopToolBarArea, m_toolBar);
}

void MainWindow::setupMenus()
{
	QMenu *fileMenu = new QMenu("&File", this);
	fileMenu->addAction(addConnectionAction);
	fileMenu->addAction(addJoystickAction);
	fileMenu->addAction(addVarMonitorAction);
	fileMenu->addAction(quitAction);
	
	menuBar()->addMenu(fileMenu);
}

void MainWindow::setupActions()
{
	quitAction = new QAction("&Quit", this);
	quitAction->setShortcut(QKeySequence::Close);
	connect(quitAction, SIGNAL(triggered(bool)),
		this, SLOT(close()));

	addConnectionAction = new QAction("Add &Connection", this);
	connect(addConnectionAction, SIGNAL(triggered(bool)),
		m_connManager, SLOT(createConnection()));
	addJoystickAction = new QAction("Add &Joystick", this);
	connect(addJoystickAction, SIGNAL(triggered(bool)),
		this, SLOT(addJoystick()));
	addVarMonitorAction = new QAction("Add &Variable Monitor", this);
	connect(addVarMonitorAction, SIGNAL(triggered(bool)),
		this, SLOT(addVarMonitor()));
}

#include "mainwindow.moc"
