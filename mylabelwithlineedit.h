#ifndef MYLABELWITHLINEEDIT_H
#define MYLABELWITHLINEEDIT_H

#include <QtWidgets>

class MyLabelWithLineEdit : public QWidget
{
    Q_OBJECT
public:
    MyLabelWithLineEdit(QWidget* = nullptr);

    MyLabelWithLineEdit(const MyLabelWithLineEdit&)             = delete;
    MyLabelWithLineEdit(MyLabelWithLineEdit&&)                  = delete;
    MyLabelWithLineEdit& operator=(const MyLabelWithLineEdit&)  = delete;
    MyLabelWithLineEdit& operator=(MyLabelWithLineEdit&&)       = delete;

    QString         getText() const;
    void            init(const QString&, const QString&);
    void            setReadOnly(bool value);

private:
    QLabel*         m_label;
    QVBoxLayout*    m_layout;
    QLineEdit*      m_lineEdit;

};

#endif // MYLABELWITHLINEEDIT_H
