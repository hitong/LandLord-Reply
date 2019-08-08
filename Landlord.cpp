#include "Landlord.h"
#pragma execution_character_set("utf-8")

Landlord::Landlord(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setAcceptDrops(true);
	openDir = new QAction(QIcon("Resources/tools/open.png"), tr("Open Dir"), this);
	ui.toolBar->addAction(openDir);
	openAction = new QAction(QIcon("Resources/tools/open.png"), tr("Open File"), this);
	ui.toolBar->addAction(openAction);
	ui.toolBar->addSeparator();

	connect(openDir, &QAction::triggered, this, [&] {
		if (choisePlayDir()) {
			delTimer();
			showDirMenu();
			playGame();
		}
	});
	connect(openAction , &QAction::triggered, this, [&] {
		if (choiseFile()) {
			delTimer();
			clearAll();
			showAll();
			showFileMenu();
		}
	});

	beginAction = new QAction(QIcon("Resources/tools/begin.png"), tr("First Step"), this);
	ui.toolBar->addAction(beginAction);
	lastAction = new QAction(QIcon("Resources/tools/last.png"), tr("Last Step"), this);
	ui.toolBar->addAction(lastAction);
	nextAction = new QAction(QIcon("Resources/tools/next.png"), tr("Next Step"), this);
	ui.toolBar->addAction(nextAction);
	endAction = new QAction(QIcon("Resources/tools/end.png"), tr("End Step"), this);
	ui.toolBar->addAction(endAction);
	ui.toolBar->addSeparator();

	connect(beginAction, &QAction::triggered, this, [&] {firstStep(); });
	connect(endAction, &QAction::triggered, this, [&] {	lastStep();	});
	connect(nextAction, &QAction::triggered, this, [&] {showNextStep(); });
	connect(lastAction, &QAction::triggered, this, [&] {showPreStep();	});

	speedS = new QAction("Slower", this);
	ui.toolBar->addAction(speedS);
	speedF = new QAction("Faster", this);
	ui.toolBar->addAction(speedF);
	connect(speedS, &QAction::triggered, this, [&] {
		if (timer) {
			int speed = timer->interval();
			if (speed < 100000) {
				speed += 500;
				timer->setInterval(speed);
			}
		}
	});
	connect(speedF, &QAction::triggered, this, [&] {
		if (timer) {
			int speed = timer->interval();
			if (speed > 700) {
				speed -= 500;
				timer->setInterval(speed);
			}
		}
	});
	disableDirAction(true);
	disableFileAction(true);

	bidCard = new QWidget(ui.bidCard);
	match.players[0].cards_widget = new QWidget(ui.player_A);
	match.players[1].cards_widget = new QWidget(ui.player_B);
	match.players[2].cards_widget = new QWidget(ui.player_C);

	match.players[0].table_widget = new QWidget(ui.path_A);
	match.players[1].table_widget = new QWidget(ui.path_B);
	match.players[2].table_widget = new QWidget(ui.path_C);

	match.players[0].arror = ui.arror_A;
	match.players[1].arror = ui.arror_B;
	match.players[2].arror = ui.arror_C;
}

void Landlord::dropEvent(QDropEvent* e) {
	QList<QUrl> urls = e->mimeData()->urls();
	if (urls.isEmpty())
		return;
	
	std::vector<QString> tmp;
	for (auto& url: urls) {
		if (url.fileName().split(".").last().compare("pbn") == 0) {
			tmp.push_back(url.toLocalFile());
		}
	}
	
	if (tmp.empty()) {
		return;
	}
	dirFile.clear();
	delTimer();
	for (auto&& a:tmp) {
		dirFile.emplace_back(std::move(a));
	}
	if(tmp.size() == 1){
		showFileMenu();
		clearAll();
		readAndDelFile(QFile(dirFile[0]));
		showAll();
	}
	else {
		std::sort(dirFile.begin(),dirFile.end());
		showDirMenu();
		playGame();
	}
}

void Landlord::dragEnterEvent(QDragEnterEvent* e)
{
	//if (e->mimeData()->hasFormat("pbn/uri-list")) //ֻ�ܴ��ı��ļ�
		e->acceptProposedAction();
}

bool Landlord::choisePlayDir()
{
	QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	QDir dir(path);
	if (!dir.exists())
	{
		return false;
	}
	dir.setFilter(QDir::Files);
	QFileInfoList fileList = dir.entryInfoList();
	std::vector<QString> tmp;
	for (int i = 0; i < fileList.size();i++) {
		if (fileList[i].suffix().compare("pbn") == 0) {
			tmp.push_back(fileList[i].absoluteFilePath());
		}
	}
	if (tmp.empty()) {
		return false;
	}
	clearAll();
	dirFile.clear();
	for (auto&& v: tmp) {
		dirFile.emplace_back(v);
	}
	return true;
}

bool Landlord::choiseFile()
{
	QFileDialog* fileDialog = new QFileDialog(this);//����һ��QFileDialog���󣬹��캯���еĲ�������������ӡ�
	fileDialog->setWindowTitle(tr("Open"));//�����ļ�����Ի���ı���
	fileDialog->setAcceptMode(QFileDialog::AcceptOpen);//�����ļ��Ի���Ϊ��ģʽ
	fileDialog->setFileMode(QFileDialog::AnyFile);//�����ļ��Ի��򵯳���ʱ����ʾ�κ��ļ����������ļ��л����ļ�
	fileDialog->setViewMode(QFileDialog::Detail);//�ļ�����ϸ����ʽ��ʾ����ʾ�ļ�������С���������ڵ���Ϣ��
	QStringList filters;
	filters << "pbn files (*.pbn)";
	//<< "Any files (*)";
	fileDialog->setNameFilters(filters);//�������������
	fileDialog->setGeometry(10, 30, 300, 200);//�����ļ��Ի������ʾλ��
	fileDialog->setDirectory(".");//�����ļ��Ի����ʱ��ʼ�򿪵�λ��
	//fileDialog->setFilter(QDir::Filte));//�����ļ����͹�����
	if (fileDialog->exec() == QDialog::Accepted) {//ע��ʹ�õ���QFileDialog::Accepted����QDialog::Accepted,����QFileDialog::Accept
		QString path = fileDialog->selectedFiles()[0];//�õ��û�ѡ����ļ���
		QFile file(path);//����һ���ļ����󣬴洢�û�ѡ����ļ�
		readAndDelFile(file);//��ȡ�ļ��е�����
		if (!file.open(QIODevice::ReadWrite)) {
			return false;
		}
		return true;
	}
	return false;
}

void Landlord::showFileMenu()
{
	disableDirAction(true);
	disableFileAction(false);
}

void Landlord::showDirMenu()
{
	disableDirAction(false);
	disableFileAction(true);
}

void Landlord::disableFileAction(bool set)
{
	nextAction->setDisabled(set);
	lastAction->setDisabled(set);
	beginAction->setDisabled(set);
	endAction->setDisabled(set);
}

void Landlord::disableDirAction(bool set)
{
	speedF->setDisabled(set);
	speedS->setDisabled(set);
}

void Landlord::delTimer()
{
	if (timer) {
		delete timer;
		timer = nullptr;
	}
}

void Landlord::showAll()
{
	ui.label_A->setText("Player A:");
	ui.label_B->setText("Player B:");
	ui.label_C->setText("Player C:");
	showCards();
	showBid();
	showBidCards();
}

void Landlord::playGame()
{
	if (dirFile.empty()) {
		return;
	}
	timer = new QTimer();
	readAndDelFile(QFile(dirFile[0]));
	dir_idx = 1;
	clearAll();
	showAll();
	connect(timer, &QTimer::timeout,this, [&]() {
		if (play_stat == READY) {
			if (match.recodes_idx_cur != match.recodes_idx_end) {
				showNextStep();
			}
			else {
				play_stat = BAD;
				dir_idx++;
			}
		}
		else {
			if (dir_idx < dirFile.size()) {
				readAndDelFile(QFile(dirFile[dir_idx]));
				clearAll();
				showAll();
			}
			else {
				dir_idx = 0;
			}
		}
	});
	timer->start(std::chrono::milliseconds(2000));
}

void Landlord::stopPlay()
{
	timer->stop();
}

void Landlord::clearAll()
{
	for (auto& p: match.players) {
		p.cards_widget = createNewWidget(p.cards_widget);
		p.table_widget = createNewWidget(p.table_widget);
		p.arror->setText("");
	}
	for (auto bid:bidList) {
		delete bid;
	}
	ui.label_A->setText("");
	ui.label_B->setText("");
	ui.label_C->setText("");
	bidCard = createNewWidget(bidCard);
	bidList.clear();
//	ui.bid_result->children();
}

void Landlord::readAndDelFile(QFile& file)
{
	ui.file_path->setText(file.fileName());
	play_stat = BAD;
	if (file.open(QIODevice::ReadOnly)) {//ֻ����ʽ
		match.reset();
		/*Data_Ty name = file.fileName().toStdString();
		Data_Ty real_name;
		int start = name.find_last_of('/') + 1;
		int end = name.find_last_of('.');
		bool flag = true;
		for (int i = start; i < end;i++) {
			if (flag ) {
				if (name[i] != ' ' && name[i] != '-') {
					match.name[0].push_back(name[i]);
				}
				else {
					flag = false;
				}
			}
			else {
				match.name[1].push_back(name[i]);
			}
		}*/
		QTextStream stream(&file);
	//	ս��1A ս��2A ս��1B ս��2B
		QString line;
		int n = 1;
		while (!stream.atEnd()) {
			line = stream.readLine(); // һ��һ�ж�ȡ����������/n����һ���ı���Ҳ������readAll()ȫ����ȡ
			//cout << line.toStdString()<<endl; //�������Ӧ�ó�����������ڡ�
			match.analysisData(std::move(line.toStdString()));
			n++;
		}
		file.close();
		play_stat = READY;
	}
}

void Landlord::drwaCard(const int cardValue, int x, QWidget* w)
{
	QString path{ "Resources/pokers/" + QString::number(cardValue) + ".jpg" };
	QPushButton* tmp = new QPushButton(w);
	tmp->setGeometry(x, 0, CardWeight, CardHigh);
	tmp->setStyleSheet("QPushButton{border-image:url(:" + path + ");}");
	tmp->show();
}

void Landlord::showCards()
{
	QString path;
	for (int i = 0; i < 3;i++) {
		match.players[i].cards_widget = createNewWidget(match.players[i].cards_widget);
		for (int j = 0; j < match.players[i].handCards_.size();j++) {
			drwaCard(match.players[i].handCards_[j], CardGap * j,  match.players[i].cards_widget);
		}
	}
}

void Landlord::showBid()
{
	QLabel* p;
	for (auto& v:match.bidRecodes) {
		Data_Ty tmp = "player";
		tmp += v.first;
		tmp += ":  ";
		tmp += v.second;
		p = new QLabel(QString::fromStdString(tmp));
		bidList.emplace_back(p);
		ui.bidList->addWidget(p);
	}
	Data_Ty landlord = "landlord: ";
	landlord += match.landlord;
	p = new QLabel(QString::fromStdString(landlord));
	bidList.emplace_back(p);
	ui.bidList->addWidget(p);
}

void Landlord::showNextStep()
{
	match.nextStep();
	showCards();
	showCardsOnTable();
}

void Landlord::showPreStep()
{
	 match.preStep();
	 showCards();
	 showCardsOnTable();
}

void Landlord::showBidCards()
{
	bidCard = this->createNewWidget(bidCard);
	for (int i = 0; i < match.bidCards.size(); i++) {
		drwaCard(match.bidCards[i],CardGap * i, bidCard);
	}
}

void Landlord::showCardsOnTable()
{
	//if (match.recodes_idx_cur >= 3) {
	//	if (match.playRecodes[match.recodes_idx_cur - 2].second[0] == -1 &&
	//		match.playRecodes[match.recodes_idx_cur - 3].second[0] == -1) {
	//		
	//	}
	//}
	for (int i = 0; i < 3; i++) {
		match.players[i].arror->setText("");
		match.players[i].table_widget = createNewWidget(match.players[i].table_widget);
	}
	if (match.recodes_idx_cur == 0) {
		return;
	}
	int i;
	for (i = match.recodes_idx_pre_cur; i < match.recodes_idx_cur;i++) {
		match.players[match.playRecodes[i].first].table_widget = 
			createNewWidget(match.players[match.playRecodes[i].first].table_widget);
		
		for (int j = 0; j < match.playRecodes[i].second.size(); j++) {
			drwaCard(match.playRecodes[i].second[j], CardGap * j, match.players[match.playRecodes[i].first].table_widget);
		}
	}
	match.players[match.playRecodes[i-1].first].arror->setText(QString::fromUtf8("��"));
}

void Landlord::firstStep()
{
	match.firstStep();
	showCards();
	showCardsOnTable();
}

void Landlord::lastStep()
{
	match.lastStep();
	showCards();
	showCardsOnTable();
}

void Landlord::mousePressEvent(QMouseEvent* e)
{
	if (match.bidRecodes.empty()) {
		return;
	}
	if (e->button() == Qt::LeftButton) {
		showNextStep();
	}
	else if (e->button() == Qt::RightButton) {
		showPreStep();
	}
}

QWidget* Landlord::createNewWidget(QWidget* widget)
{
	auto f = widget->parent();
	delete widget;
	widget = new QWidget((QWidget*)f);
	widget->setGeometry(0, 0, 10000, 10000);//���ؼ����ƴ�С
	widget->show();
	return widget;
}

