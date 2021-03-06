#pragma once

#include <QLineEdit>
#include <QPointer>
#include <View/Parameters/BoundWidget.h>
#include <ViewModel/Parameters/TextModel.h>

// Line edit bound to a TextModel

class BoundText : public QLineEdit, public BoundWidget
{
    Q_OBJECT
    Q_INTERFACES(BoundWidget)

public:
    BoundText(TextModel* model, QWidget* parent = nullptr);
    virtual const ParameterModel* getModel() const override;
    virtual void updateFromModel() override;

private:
    QPointer<TextModel> m_model;
};
