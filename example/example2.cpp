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

#include <qapplication.h>
#include <Qtfe.h>

#include "QImageTweak.h"

int main(int argc, char** argv)
{
	QApplication application(argc,argv);

	Qtfe *editor = new Qtfe;
	editor->show();

	QImageTweak * qit = new QImageTweak(editor);
	qit->show();

	return application.exec();
}
