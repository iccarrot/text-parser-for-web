#include "mylabelwithlineedit.h"

MyLabelWithLineEdit::MyLabelWithLineEdit(QWidget* parent) : QWidget (parent)
{
    m_label     = new QLabel();
    m_layout    = new QVBoxLayout();
    m_lineEdit  = new QLineEdit();
}

QString MyLabelWithLineEdit::getText() const
{
    return m_lineEdit->text();
}

void MyLabelWithLineEdit::init(const QString& labelText, const QString& exampleText)
{
    m_label->setText(labelText);
    m_lineEdit->setText(exampleText);

    m_layout->addWidget(m_label);
    m_layout->addWidget(m_lineEdit);

    setLayout(m_layout);
}

void MyLabelWithLineEdit::setReadOnly(const bool value)
{
    m_lineEdit->setReadOnly(value);
}
