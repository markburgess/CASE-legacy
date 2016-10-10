#include <XYCell.h>

XYCell::XYCell()
{
}

/*****************************************************************************/

XYCell::~XYCell()
{
}

/*****************************************************************************/

void XYCell::SetupRequester(Req *requester)
{
 angle.InitWidgets(requester, "Angle");
}

/*****************************************************************************/

void XYCell::ReadWidgets(Req *requester)
{
 angle.ReadWidgets(requester, "Angle");
}

/*****************************************************************************/

void XYCell::UpdateWidgetValue(Req *requester)
{
 angle.UpdateWidgetValue(requester, "Angle");
}

/*****************************************************************************/

double XYCell::GetAngle()
{
 return angle.GetValue();
}

/*****************************************************************************/

void XYCell::SetAngle(double new_angle)
{
 if (new_angle > PIx2)
    {
    angle.SetValue(new_angle - PIx2);
    }
 else if (new_angle < 0)
    {
    angle.SetValue(new_angle + PIx2);
    }
 else
    {
    angle.SetValue(new_angle);
    }
}

/*****************************************************************************/
