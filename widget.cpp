#include "widget.h"
#include "ui_widget.h"


#define THREE_LAYOUT




#include <QMenu>
#include <vector>
// 全局下设置增加菜单
QAction *NewAction;
QAction *InsertAction;
QAction *DeleteAction;
QListWidget  *list_left;
QListWidget  *list_right;
QSplitter *g_sp;




std::vector<QListWidgetItem *>  item_vector;
std::unordered_map<std::string, std::string> g_strategy_property;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{

#ifdef THREE_LAYOUT


    QPalette pal = QPalette();

    // set black background
    // Qt::black / "#000000" / "black"
    pal.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    //菜单根指针
    QMenuBar * pMennuBar = new QMenuBar(this);
    //第一个菜单
    QMenu * pMenuItem = new QMenu(QStringLiteral("构建"), this);
    QMenu * pMenuItem_spae_2 = new QMenu(QStringLiteral(" "), this);
    //QMenu * pMenuItem_spae_3   = new QMenu(QStringLiteral("3 "), this);
    QMenu * pMenuItem_spae_4 = new QMenu(QStringLiteral(" "), this);
    QAction *p_test1 = new QAction("测试", this);
    QAction *p_test2 = new QAction("测试2", this);
    pMenuItem->addAction(p_test1);
    pMenuItem->addAction(p_test2);
    pMennuBar->addMenu(pMenuItem);
   // pMennuBar->addMenu(pMenuItem_spae_3);
   // pMennuBar->addMenu(pMenuItem_spae_3);



    QMenu * fileMenu = pMennuBar->addMenu("菜单1");
//    bar->setVisible(false);   // 隐藏顶部菜单栏

    // 添加子菜单
     NewAction = fileMenu->addAction("启动策略");
     InsertAction = fileMenu->addAction("停止策略");
     //DeleteAction = fileMenu->addAction("删除策略");

     connect(NewAction,&QAction::triggered,this, &Widget::call_menu);


    QVBoxLayout* layout_list = new QVBoxLayout(this);


    QSpacerItem *pSpacer = new QSpacerItem(20, 20 , QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout_list->addSpacerItem(pSpacer);
    QSplitter *sp = new QSplitter(Qt::Orientation::Horizontal, this);
    g_sp = sp;
    sp->setStyleSheet("QSplitter:handle{background-color:green}");
    list_left = new QListWidget(sp);
    QListWidget  *list_middle = new QListWidget(sp);
    list_right = new QListWidget(sp);



    list_left->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(list_left, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_listLeft_customContextMenuRequested(const QPoint &)));


    sp->addWidget(list_left);
    lef_list_view_op(list_left, sp);

    sp->addWidget(list_right);

    layout_list->addWidget(sp);

    layout_list->setContentsMargins(0, 0, 0, 0);
    sp->widget(0)->setVisible(false);

#endif



    connect(list_left, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(single_click(QListWidgetItem*)));
    connect(list_left, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(double_click()));
    connect(list_left, SIGNAL(click), this, SLOT(double_click()));


    ui->setupUi(this);



    //setMenuBar(pMennuBar);
}

void Widget::lef_list_view_op( QListWidget * left, QSplitter * sp){

    for (int i = 0 ; i < 10 ; i++){

        //const QLabel *q = new QLabel( QString(i) ,left);
        std::string item_name = "strategy" + std::to_string(i);
        QListWidgetItem *item = new  QListWidgetItem();
        item->setText(item_name.c_str());
        //设定item 属性为可编辑状态
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable
                                |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        item_vector.push_back(item);
        left->addItem(item);
        strategy_attribute_init(item_name);
        g_strategy_property[item_name] = item_name;
        if( i == 0){
            QtTreePropertyBrowser *tree = create_property(item_name);
            sp->addWidget(tree);

        }
        else{

        }
    }

}
//创建一个属性框口
QtTreePropertyBrowser * Widget::create_property(std::string name){
   // QVBoxLayout * layout = new QVBoxLayout();
    QtVariantPropertyManager *pMananger =  new QtVariantPropertyManager();
    m_Factory = new QtVariantEditorFactory();
    QtTreePropertyBrowser *tree = new QtTreePropertyBrowser();
    w = tree;
    m_Manager = pMananger;
    tree->setFactoryForManager(m_Manager, m_Factory);
    group_broswer_wrapper_ = new QtGroupBrowserWrapper(pMananger);
    connect(pMananger, &QtVariantPropertyManager::valueChanged, this, &Widget::PropertyValueChanged);

    //ui->widget->setFactoryForManager(pMananger, pEditor);

    QtProperty *pItem = pMananger->addProperty(QtVariantPropertyManager::groupTypeId(), "group_test_1");
    QtVariantProperty *item = pMananger->addProperty(QVariant::Int, QStringLiteral("X"));
    item->setValue(100);
    pItem->addSubProperty(item);

    item = pMananger->addProperty(QVariant::Double, QStringLiteral("Double"));
    item->setValue(3.3338);
    pItem->addSubProperty(item);

    tree->addProperty(pItem);
    group_broswer_wrapper_->push_back(pItem);


    QtProperty *pItem2 = pMananger->addProperty(QtVariantPropertyManager::groupTypeId(), "group_test_2");
    item = pMananger->addProperty(QVariant::Int, QStringLiteral("Y"));
    item->setValue(7);
    pItem2->addSubProperty(item);

    item = pMananger->addProperty(QVariant::String, QStringLiteral("StrAttribute"));
    item->setValue(QStringLiteral("hello world"));
    pItem2->addSubProperty(item);

    tree->addProperty(pItem2);
    group_broswer_wrapper_->push_back(pItem2);

    group_broswer_wrapper_->set_value("group_test_1", "X", 66);
    return tree;
}

void Widget::single_click(QListWidgetItem* item)
{

    for(int i = 0 ; i < item_vector.size(); i++){
        if( item_vector[i]->isSelected() ){

            std::string  curr_name = item_vector[i]->text().toStdString();
            curr_strategy_ = curr_name;
            update_attribute(curr_name);
            if( g_strategy_property[curr_name].size() > 0){
                qDebug("x pos");
                qDebug(std::to_string(g_sp->pos().x()).c_str());

                std::string item_name = item->text().toStdString() + " selected !";
                QListWidgetItem *item = new  QListWidgetItem();
                item->setText(item_name.c_str());
                //设定item 属性为可编辑状态
                //item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable|Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
                item->setFlags(item->flags()&~Qt::ItemIsSelectable);


                item_vector.push_back(item);
                list_right->addItem(item);
            }
            else{
                qDebug("miss Item");
            }
        }
        else{
            item_vector[i]->setBackground(QBrush( Qt::white));
        }
    }
    item->setBackground( QBrush( Qt::red) );

}

void Widget::double_click()
{

    qDebug("double");

}

void Widget::strategy_attribute_init(std::string name)
{
    //std::unordered_map<std::string, std::vector< std::unordered_map<std::string, QVariant> >>
     std::unordered_map<std::string, QVariant> group_test_1;
     group_test_1["X"] = 1;
     group_test_1["Double"] = 0.33;
     std::unordered_map<std::string, QVariant> group_test_2;
     group_test_2["Y"] = 3;
     group_test_2["StrAttribute"] = "somethingStr";


    std::unordered_map<std::string, std::unordered_map<std::string, QVariant>> group_map;
    group_map["group_test_1"] = group_test_1;
    group_map["group_test_2"] = group_test_2;

    strategy_attribute_[name] = group_map;

}

void Widget::update_attribute(std::string strategy_name)
{
    auto group_map =  strategy_attribute_[strategy_name];
    QVariant val1 = group_map["group_test_1"]["X"];
    group_broswer_wrapper_->set_value("group_test_1", "X", val1);
    //group_broswer_wrapper_->set_value("group_test_1", "X", 44);
    QVariant vals = group_broswer_wrapper_->get_value("group_test_1", "X");
    std::cout<<"[get]" <<  vals.toInt() << std::endl;

    QVariant val2 = group_map["group_test_1"]["Double"];
    group_broswer_wrapper_->set_value("group_test_1", "Double", val2);

    QVariant val3 = group_map["group_test_2"]["Y"];
    group_broswer_wrapper_->set_value("group_test_2", "Y", val3);

    QVariant val4 = group_map["group_test_2"]["StrAttribute"];
    group_broswer_wrapper_->set_value("group_test_2", "StrAttribute", val4);

}

void Widget::call_menu()
{
    std::string item_name = curr_strategy_ + " start !";

    QListWidgetItem *item = new  QListWidgetItem();
    item->setText(item_name.c_str());
    item->setFlags(item->flags()&~Qt::ItemIsSelectable);
    list_right->addItem(item);

}


Widget::~Widget()
{
    delete ui;
}


void Widget::on_listLeft_customContextMenuRequested(const QPoint &pos)
{

    QListWidgetItem  * curr_item= list_left->currentItem();
    if (curr_item){
        if(curr_item->text().toStdString().size()< 2 ||  (!curr_item->isSelected()) ) return ;
        std::string curr_name = curr_item->text().toStdString();

    }
    else{
        return ;
    }
        Q_UNUSED(pos);

        // 新建Menu菜单
        QMenu *ptr = new QMenu(this);

        // 添加Actions创建菜单项
        ptr->addAction(NewAction);
        // 添加一个分割线
        ptr->addSeparator();
        ptr->addAction(InsertAction);
       // // 添加一个分割线
       // ptr->addSeparator();
       // ptr->addAction(DeleteAction);

        ptr->exec(QCursor::pos());

        delete ptr;

}

void Widget::PropertyValueChanged(QtProperty *item, const QVariant value)
{
    //std::group_
    for(auto & group : strategy_attribute_[curr_strategy_]){
        for(auto & attribute : group.second){
            if (attribute.first == item->propertyName().toStdString()){
                attribute.second = value;
            }
        }

    }
     QVariant valueS = strategy_attribute_[curr_strategy_]["group_test_2"]["StrAttribute"];
     qDebug("item changed !");
}

void Widget::on_Widget_customContextMenuRequested(const QPoint &pos)
{
    qDebug("windos pos");
}

