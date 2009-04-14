#include <QtGui>
#include "dlg_creategame.h"

DlgCreateGame::DlgCreateGame(Client *_client, QWidget *parent)
	: QDialog(parent), client(_client), msgid(0)
{
	descriptionLabel = new QLabel(tr("&Description:"));
	descriptionEdit = new QLineEdit;
	descriptionLabel->setBuddy(descriptionEdit);

	passwordLabel = new QLabel(tr("&Password:"));
	passwordEdit = new QLineEdit;
	passwordLabel->setBuddy(passwordEdit);

	maxPlayersLabel = new QLabel(tr("P&layers:"));
	maxPlayersEdit = new QLineEdit("2");
	maxPlayersLabel->setBuddy(maxPlayersEdit);

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(descriptionLabel, 0, 0);
	grid->addWidget(descriptionEdit, 0, 1);
	grid->addWidget(passwordLabel, 1, 0);
	grid->addWidget(passwordEdit, 1, 1);
	grid->addWidget(maxPlayersLabel, 2, 0);
	grid->addWidget(maxPlayersEdit, 2, 1);

	okButton = new QPushButton(tr("&OK"));
	okButton->setDefault(true);
	cancelButton = new QPushButton(tr("&Cancel"));
	
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(grid);
	mainLayout->addLayout(buttonLayout);
	
	setLayout(mainLayout);
	
	setWindowTitle(tr("Create game"));
	setFixedHeight(sizeHint().height());
	
	connect(okButton, SIGNAL(clicked()), this, SLOT(actOK()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(client, SIGNAL(responseReceived(ServerResponse *)), this, SLOT(checkResponse(ServerResponse *)));
}

void DlgCreateGame::actOK()
{
	bool ok;
	int maxPlayers = maxPlayersEdit->text().toInt(&ok);
	if (msgid)
		return;
	if (!ok) {
		QMessageBox::critical(this, tr("Error"), tr("Invalid number of players."));
		return;
	}
	msgid = client->createGame(descriptionEdit->text(),
				   passwordEdit->text(),
				   maxPlayers);
}

void DlgCreateGame::checkResponse(ServerResponse *response)
{
	if (response->getMsgId() != msgid)
		return;
	if (response->getOk())
		accept();
	else {
		QMessageBox::critical(this, tr("Error"), tr("XXX"));
		msgid = 0;
		return;
	}
}
