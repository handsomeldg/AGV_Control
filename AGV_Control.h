#pragma once

#include <QtWidgets/QWidget>
#include "ui_AGV_Control.h"

class AGV_Control : public QWidget
{
    Q_OBJECT

public:
    AGV_Control(QWidget *parent = nullptr);
    ~AGV_Control();

private:
    Ui::AGV_ControlClass ui;
};

