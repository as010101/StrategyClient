#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QMenuBar>
#include <QMenu>
#include <QListView>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSpacerItem>
#include <QLabel>
#include <QListWidget>
#include <iostream>
#include <QMainWindow>


#include <QDateTime>
#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QSize>
#include <QSizeF>
#include <QString>
#include <any>
//#include <qedit.h>
//#include <QLabel>

#include <qtpropertybrowser.h>
#include <qtpropertymanager.h>
#include <qtvariantproperty.h>
#include <qttreepropertybrowser.h>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class GroupItem{
public:
    template <typename T>
    void register_item(const std::string & str, QVariant & t){
        item_map_[str] = t;
    }

    std::any get(const std::string &key){
        return item_map_[key];
    }

    std::string get_name(){
        return group_name_;
    }
    std::string set_name(std::string str){
        group_name_ = str;
    }
private:
    std::unordered_map<std::string, QVariant> item_map_;
    std::string group_name_;
};


class StrategyGroup{

public:

    void register_group(const std::string & str){
        GroupItem group;
        group.set_name(str);
        item_map_[str] = group;
    }

    GroupItem  get(const std::string &key){
        return item_map_[key];
    }

    std::string get_name(){
        return strategy_name_;
    }
    void set_name(const std::string & str){
        strategy_name_ = str;
    }
private:
    std::unordered_map<std::string, GroupItem> item_map_;
    std::string strategy_name_;
};


class QtGroupBrowserWrapper{
public:
    QtGroupBrowserWrapper(QtVariantPropertyManager *p): mananger_(p){

    }


    void push_back(QtProperty *p){
        item_vec_.push_back(p);
    }
    QVariant get_value(const std::string & group_item, const std::string & sub_item_name ){
        for(auto & ele : item_vec_){
            if ( ele->propertyName().toStdString() == group_item){
                for(auto & sub_ele : ele->subProperties()){
                    if(sub_ele->propertyName().toStdString() == sub_item_name){
                        return  mananger_->value(sub_ele);
                    }

                }
            }
        }
    }

    void set_value(const std::string & group_item, const std::string & sub_item_name ,const QVariant & value ){
        for(auto & ele : item_vec_){
            if ( ele->propertyName().toStdString() == group_item){
                for(auto & sub_ele : ele->subProperties()){
                    if(sub_ele->propertyName().toStdString() == sub_item_name){
                         mananger_->setValue(sub_ele, value);
                    }

                }
            }
        }
    }

private:
    std::vector<QtProperty *> item_vec_;
    QtVariantPropertyManager *mananger_;

};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();


    void lef_list_view_op( QListWidget * left ,QSplitter*);
    QtTreePropertyBrowser * create_property(std::string name);

public:

    QtVariantPropertyManager *m_Manager;
    QtVariantEditorFactory *m_Factory;
    QtTreePropertyBrowser *w;
public slots:
    void single_click(QListWidgetItem*);
    void double_click();
    void strategy_attribute_init(std::string name);
    void update_attribute(std::string str);
    void call_menu();
private slots:
    void on_listLeft_customContextMenuRequested(const QPoint &pos);

    void on_Widget_customContextMenuRequested(const QPoint &pos);


public slots:
    void PropertyValueChanged(QtProperty * item, const QVariant value);

private:
    Ui::Widget *ui;
    std::string  curr_strategy_;
    QtGroupBrowserWrapper  *group_broswer_wrapper_;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, QVariant> >>  strategy_attribute_;  // strategy-->  m个group -->每个group都有dict-->value

};





#endif // WIDGET_H


