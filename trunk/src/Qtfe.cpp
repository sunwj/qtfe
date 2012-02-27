/************************************************************************

Copyright (C) 2012 Eric Heitz (er.heitz@gmail.com). All rights reserved.

This file is part of Qtfe (Qt Transfer Function Editor).

Qtfe is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as 
published by the Free Software Foundation, either version 3 of 
the License, or (at your option) any later version.

Qtfe is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Qtfe.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#include "Qtfe.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QLabel>

#include <QDomDocument>
#include <QDomElement>

Qtfe::Qtfe(QWidget * parent, Qt::WindowFlags f) : 
QWidget(parent, f),
filename("New Transfer Function.xml")
{
	QVBoxLayout *layout = new QVBoxLayout;

	QGridLayout * layoutButtons = new QGridLayout;
	QPushButton * saveButton = new QPushButton("Save",this);
	QPushButton * saveAsButton = new QPushButton("Save as",this);
	QPushButton * loadButton = new QPushButton("Load", this);
	fileLabel = new QLabel(filename, this);
	layoutButtons->addWidget(saveButton, 0, 0);
	layoutButtons->addWidget(loadButton, 1, 0);
	layoutButtons->addWidget(saveAsButton, 0, 1);
	layoutButtons->addWidget(fileLabel, 1, 1);

	layout->addLayout(layoutButtons);
	QWidget * zone = new QWidget(this);
	layout->addWidget(zone);
	this->setLayout(layout);

	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
	QObject::connect(saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));
	QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
}

Qtfe::~Qtfe()
{
	for(int i=0 ; i < outputs.size() ; ++i)
		delete outputs[i];
	for(int i=0 ; i < canals.size() ; ++i)
		delete canals[i];
}

void Qtfe::addCanals(int n)
{
	for(int i=0 ; i<n ; ++i)
	{
		QtfeCanal * canal = new QtfeCanal();
		canals.push_back(canal);
		this->layout()->addWidget(canal);
		QObject::connect(canal, SIGNAL(canalChanged()), this, SLOT(canalChanged()));
	}
}

void Qtfe::addOutputs(int n)
{
	for(int i=0 ; i<n ; ++i)
	{
		QtfeOutput * output = new QtfeOutput(this);
		outputs.push_back(output);
		this->layout()->addWidget(output);
	}	
}

void Qtfe::bindCanaltoOutputR(int canal, int output)
{
	if(0 <= output && output < outputs.size())
		outputs[output]->bindCanaltoR(canal);
}

void Qtfe::bindCanaltoOutputG(int canal, int output)
{
	if(0 <= output && output < outputs.size())
		outputs[output]->bindCanaltoG(canal);
}

void Qtfe::bindCanaltoOutputB(int canal, int output)
{
	if(0 <= output && output < outputs.size())
		outputs[output]->bindCanaltoB(canal);
}

void Qtfe::bindCanaltoOutputA(int canal, int output)
{
	if(0 <= output && output < outputs.size())
		outputs[output]->bindCanaltoA(canal);
}

int Qtfe::dim() const
{
	return canals.size();
}

qreal Qtfe::evalf(int canal, qreal x) const
{
	if(0 <= canal && canal < canals.size())
		return canals[canal]->evalf(x);
	else
		return 0.0;
}

void Qtfe::canalChanged()
{
	for(int i=0 ; i < outputs.size() ; ++i)
	{
		outputs[i]->repaint();
	}
	emit functionChanged();
}

void Qtfe::save()
{
	QDomDocument doc;
	QDomElement MultiEditor;
	QFile file;
	QTextStream out;

	file.setFileName(filename);
	if (!file.open(QIODevice::WriteOnly))
		return;
	out.setDevice(&file);

	MultiEditor = doc.createElement("Qtfe");
	doc.appendChild(MultiEditor); 

	for(int i=0 ; i<canals.size() ; ++i)
	{
		QDomElement func = doc.createElement("Function");
		MultiEditor.appendChild(func);

		for(int j=0 ; j<canals[i]->getPoints().size() ; ++j)
		{
			QDomElement point = doc.createElement("point");
			func.appendChild(point);
			point.setAttribute("x", QString::number(canals[i]->getPoints()[j]->x()));
			point.setAttribute("y", QString::number(canals[i]->getPoints()[j]->y()));
		}
	}
	for(int i=0 ; i<outputs.size() ; ++i)
	{
		QDomElement output = doc.createElement("Output");
		MultiEditor.appendChild(output);
		output.setAttribute("R",  QString::number(outputs[i]->R));
		output.setAttribute("G",  QString::number(outputs[i]->G));
		output.setAttribute("B",  QString::number(outputs[i]->B));
		output.setAttribute("A",  QString::number(outputs[i]->A));
	}
	
	QDomNode noeud = doc.createProcessingInstruction("xml","version=\"1.0\"");
	doc.insertBefore(noeud,doc.firstChild());

	doc.save(out,2);

	file.close();
}

void Qtfe::saveAs()
{
	QString name = QFileDialog::getSaveFileName(0, QString(), QString(), "*.xml");
	if(name.isEmpty())
		return;
	filename = name;
	fileLabel->setText(filename);
	save();
}

void Qtfe::load()
{
	QString name = QFileDialog::getOpenFileName(0, QString(), QString(), "*.xml");
	if(name.isEmpty())
		return;
	filename = name;
	fileLabel->setText(filename);
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return;

	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();

	// block functionChanged() signal while loading
	blockSignals(true);

	for(int i=0 ; i < canals.size() ; ++i)
		delete canals[i];
	for(int i=0 ; i < outputs.size() ; ++i)
		delete outputs[i];

	canals.clear();
	outputs.clear();

	QDomElement root = doc.documentElement();
	QDomNode node = root.firstChild();

	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if (element.tagName() == "Function")
		{
			addCanals(1);
			QtfeCanal * canal = canals.back();
			QDomNodeList points = element.childNodes();
			canal->setFirstPoint(points.item(0).toElement().attributeNode("y").value().toDouble());
			canal->setLastPoint(points.item(points.length()-1).toElement().attributeNode("y").value().toDouble());

			for(uint i=1;i<points.length()-1;i++)
			{
				QDomNode point = points.item(i);

				qreal x = point.toElement().attributeNode("x").value().toDouble();
				qreal y = point.toElement().attributeNode("y").value().toDouble();
		
				canal->insertPoint(QPointF(x,y));
			}
		}
		if (element.tagName() == "Output")
		{
			QtfeOutput * output = new QtfeOutput(this);
			output->bindCanaltoR(element.attributeNode("R").value().toInt());	
			output->bindCanaltoG(element.attributeNode("G").value().toInt());
			output->bindCanaltoB(element.attributeNode("B").value().toInt());
			output->bindCanaltoA(element.attributeNode("A").value().toInt());
			outputs.push_back(output);
			this->layout()->addWidget(output);
		}

		node = node.nextSibling();
	}

	// unblock functionChanged() signal and emit
	blockSignals(false);
	emit functionChanged();
}

