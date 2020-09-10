//#include <wx/sizer.h>
#include <wx/wx.h>
#include <wx/timer.h>

#include "Calculate.h"
#include <math.h>


//class declaration
class MyFrame;

//Global Variables
//-------------------------------
double mainFrameWidth = 900;
double mainFrameHeight = 600;
double xAxisMax = 20;
double yAxisMax = 20;
unsigned int numX_Intervals = 20;
unsigned int numY_Intervals = 20;
//Sizer used for frame (to keep track of size globally)
wxBoxSizer* sizer;
//Colors to use
wxColor white(225, 255, 255, 255);
wxColor black(0, 0, 0, 255);
//the main frame
MyFrame* frame;
//string in text input box
wxString inputText;

std::string equationM = "y = 0";

//-------------------------------

class BasicDrawPane;
double Function(double x);

class RenderTimer : public wxTimer
{
    BasicDrawPane* pane;
public:
    RenderTimer(BasicDrawPane* pane);
    void Notify();
    void start();
};


class BasicDrawPane : public wxPanel
{  
public:
    BasicDrawPane(wxFrame* parent);
	
    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render( wxDC& dc );
    
    DECLARE_EVENT_TABLE()
};

class MyApp: public wxApp
{
    bool OnInit();
public:
//frame pointer was here.   
};


RenderTimer::RenderTimer(BasicDrawPane* pane) : wxTimer()
{
    RenderTimer::pane = pane;
}

void RenderTimer::Notify()
{
    pane->Refresh();
}

void RenderTimer::start()
{
    wxTimer::Start(300);
}

IMPLEMENT_APP(MyApp)

class MyFrame : public wxFrame
{

private:

    RenderTimer* timer;
    BasicDrawPane* drawPane;

//Eventually move the variables here
/*
    double mainFrameWidth = 900;
    double mainFrameHeight = 600;
    double xAxisMax = 20;
    double yAxisMax = 20;
    unsigned int numX_Intervals = 20;
    unsigned int numY_Intervals = 20;
    //Sizer used for frame (to keep track of size globally)
    wxBoxSizer* sizer;
    //Colors to use
    wxColor white(225, 255, 255, 255);
    wxColor black(0, 0, 0, 255);
    //the main frame
    MyFrame* frame;
    //string in text input box
    wxString inputText;
*/


    
public:

    MyFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Grapher"), wxPoint(50,50), wxSize(mainFrameWidth, mainFrameHeight))
    {
        sizer = new wxBoxSizer(wxHORIZONTAL);
        drawPane = new BasicDrawPane( this );
        sizer->Add(drawPane, 1, wxEXPAND);
        SetSizer(sizer);
        Center();

        MainEditBox = new wxTextCtrl(this, wxID_HIGHEST, wxT(""), wxPoint(50, mainFrameHeight - 60), wxSize(200, 40),
            wxTE_MULTILINE | wxTE_RICH , wxDefaultValidator, wxTextCtrlNameStr);
        
        timer = new RenderTimer(drawPane);
        Show();
        timer->start();
    }

    wxTextCtrl *MainEditBox; //added

    void textBoxResize()
    {
        //Move the textBox to keep in frame and resize to frame
        wxPoint mainTextBoxPos(1 * (mainFrameWidth/30), 19 * (mainFrameHeight/20));
        MainEditBox->Move(mainTextBoxPos);
        MainEditBox->SetSize(2 * (mainFrameWidth/10), 1 * (mainFrameHeight/25));

        //Resize Font approriate to frame height
        wxFont font = MainEditBox->GetFont();
        font.SetPointSize((14.0/600.0) * mainFrameHeight);
        if (font.GetPointSize() != 0) //Protects from crash
        {
            MainEditBox->SetFont(font);
        }
     
    }

    ~MyFrame()
    {
        delete timer;
    }

    void onClose(wxCloseEvent& evt)
    {
        timer->Stop();
        evt.Skip();
    }
    DECLARE_EVENT_TABLE()

};


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
    frame = new MyFrame();
    frame->Show(true);
    SetTopWindow(frame);

    return true;
} 


BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()



BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}


void BasicDrawPane::paintEvent(wxPaintEvent& evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

//Draws Function
void DrawFunction(wxDC& dc, unsigned int xAxisMax, unsigned int yAxisMax,
    unsigned int numX_Intervals, unsigned int numY_Intervals)
{

    //Point of graphs origin
    wxPoint origin(2 * (mainFrameWidth/10), 8 * (mainFrameHeight/10));

    //Create Axis Length Variables
    double xAxisPixelLength = (9 * (mainFrameWidth/10)) - (2 * (mainFrameWidth/10));
    double yAxisPixelLength = (8 * (mainFrameHeight/10)) - (mainFrameHeight/10);

    //Find Interval Pixel Size
    double xIntervalPixelSize = xAxisPixelLength / numX_Intervals;
    double yIntervalPixelSize = yAxisPixelLength / numY_Intervals;

    //Find numerical interval size
    double xIntervalSize = xAxisMax / numX_Intervals;
    double yIntervalSize = yAxisMax / numY_Intervals;

    //Number of circles to plot (draw)
    double numPointsToPlot = xAxisPixelLength;

    //xAxis ratio multiplier
    double xAxisMultiplier = (xAxisPixelLength * 1.0) / (xAxisMax * 1.0);

    //yAxis ratio multiplier
    double yAxisMultiplier = (yAxisPixelLength * 1.0) / (yAxisMax * 1.0);

    //Setup pen to draw
    wxPen pen;
	pen.SetColour(255,255,255);
	pen.SetWidth(1);
	pen.SetStyle(wxSOLID);
	dc.SetPen(pen);

    //Label the x axis and x-grid lines
    double currentInterval = 0;
    for (unsigned int i = 0; i <= numX_Intervals; i++)
    {

        //Draw correct interval
        unsigned int temp = currentInterval;
        wxString LcurrentIntervalwxString( std::to_string(temp) );

        // offset is how many pixels below the xaxis the intervals are drawn
        double offsetX = 3;
        double offsetY = 3;
        dc.DrawText(wxT(currentIntervalwxString), origin.x + (i * xIntervalPixelSize) - offsetX, origin.y + offsetY);

        //Set new color for grid lines
        pen.SetColour(25,25,25);
	    dc.SetPen(pen);

        //Point for end of x-axis (x=0)
        wxPoint gridStart(origin.x + (i * xIntervalPixelSize), origin.y);
        wxPoint gridEnd(origin.x + (i * xIntervalPixelSize), origin.y - yAxisPixelLength);

        //Draw y-grid lines
        if (i != 0)
        {
            dc.DrawLine(gridStart, gridEnd);
        }

        //Increase interval
        currentInterval = currentInterval + xIntervalSize;
    }

    //Label the y axis and y-grid lines
    currentInterval = 0;
    for (unsigned int i = 0; i <= numY_Intervals; i++)
    {

        //Draw correct interval
        unsigned int temp = currentInterval;
        wxString LcurrentIntervalwxString( std::to_string(temp) );

        //OffsetX is how many pixels left of the yaxis the intervals are drawn
        //OffsetY is how many pixels up the yaxis the intervals are drawn
        // (OffsetY is to aline yIntervals with grid lines)
        //We will account for some multiple digits
        double offsetX = 17;
        double offsetY = 8;
        if (currentInterval < 10)
        {
            offsetX = 14;
        }
        if (currentInterval >= 10)
        {
            offsetX = 19;
        }
        if (currentInterval >= 100)
        {
            offsetX = 26;
        }
        if (currentInterval >= 1000)
        {
            offsetX = 32;
        }

        if (i != 0)
        {
            dc.DrawText(wxT(currentIntervalwxString), origin.x - offsetX, origin.y - (i * yIntervalPixelSize) - offsetY);
        }

        //Set new color for grid lines
        pen.SetColour(25,25,25);
	    dc.SetPen(pen);

        //Point for end of y-axis (y=0)
        wxPoint gridStart(origin.x, origin.y - (i * yIntervalPixelSize));
        wxPoint gridEnd(origin.x + xAxisPixelLength, origin.y - (i * yIntervalPixelSize));

        //Draw y-grid lines
        if (i != 0)
        {
            dc.DrawLine(gridStart, gridEnd);
        }

        //Increase interval
        currentInterval = currentInterval + yIntervalSize;

    }

    //Plot (draw) all anti-aliasing points on the functions line
    double currentX_Value = 0;
    double fOfX = 0;
    for (int i = 0; i < numPointsToPlot; i++)
    {
        //Compute the point for plotting
        fOfX = Function(currentX_Value);
        double xPixelValue = (xAxisMultiplier * currentX_Value) + origin.x;
        double yPixelValue = origin.y - (yAxisMultiplier * fOfX);
        wxPoint pointToPlot(xPixelValue, yPixelValue);

        //Anti-Alias Points
        //Right/Left
        wxPoint pointToPlotRight(pointToPlot.x + 1, pointToPlot.y);
        wxPoint pointToPlotLeft(pointToPlot.x - 1, pointToPlot.y);
        wxPoint pointToPlotRight2(pointToPlot.x + 2, pointToPlot.y);
        wxPoint pointToPlotLeft2(pointToPlot.x - 2, pointToPlot.y);
        wxPoint pointToPlotRight3(pointToPlot.x + 3, pointToPlot.y);
        wxPoint pointToPlotLeft3(pointToPlot.x - 3, pointToPlot.y);
        wxPoint pointToPlotRight4(pointToPlot.x + 4, pointToPlot.y);
        wxPoint pointToPlotLeft4(pointToPlot.x - 4, pointToPlot.y);

        //Plot Alias points1
        pen.SetColour(200,200,200);
	    dc.SetPen(pen);
        dc.DrawPoint(pointToPlotRight);
        dc.DrawPoint(pointToPlotLeft);

        //Plot Alias points2
        pen.SetColour(100,100,100);
	    dc.SetPen(pen);
        dc.DrawPoint(pointToPlotRight2);
        dc.DrawPoint(pointToPlotLeft2);

        //Plot Alias points3
        pen.SetColour(50,50,50);
	    dc.SetPen(pen);
        dc.DrawPoint(pointToPlotRight3);
        dc.DrawPoint(pointToPlotLeft3);

        //Plot Alias points4
        pen.SetColour(20,20,20);
	    dc.SetPen(pen);
        dc.DrawPoint(pointToPlotRight4);
        dc.DrawPoint(pointToPlotLeft4);


        //1.0 is to insure the type is used as a double
        //Increment to the next x value
        currentX_Value = currentX_Value + (1.0 * (xAxisMax * 1.0) / (xAxisPixelLength * 1.0) );

    }
    //Replot main points over the aliasing points
    currentX_Value = 0;
    fOfX = 0;
    for (int i = 0; i < numPointsToPlot; i++)
    {
        //Compute the point for plotting
        fOfX = Function(currentX_Value);
        double xPixelValue = (xAxisMultiplier * currentX_Value) + origin.x;
        double yPixelValue = origin.y - (yAxisMultiplier * fOfX);
        wxPoint pointToPlot(xPixelValue, yPixelValue);

        //Plot main point
        pen.SetColour(255,255,255);
	    dc.SetPen(pen);
        dc.DrawPoint(pointToPlot);

        // 4.0 is the pixels we skip each x value //changed to 1
        currentX_Value = i * (1.0 * (xAxisMax * 1.0) / (xAxisPixelLength * 1.0) );
    }

    //---------------FOR DEBUFGGING----------- [F(X) - 1] -------
        //Demo (to see anti aliasing comparison)
        //Plot (draw) all main points on the functions line
        /*
        currentX_Value = 0;
        fOfX = 0;
        for (int i = 0; i < numPointsToPlot; i++)
        {
            //Compute the point for plotting
            fOfX = Function(currentX_Value) - 1;
            double xPixelValue = (xAxisMultiplier * currentX_Value) + origin.x;
            double yPixelValue = origin.y - (yAxisMultiplier * fOfX);
            wxPoint pointToPlot(xPixelValue, yPixelValue);

            //Plot main point
            pen.SetColour(255,255,255);
            dc.SetPen(pen);
            dc.DrawPoint(pointToPlot);

            // 4.0 is the pixels we skip each x value //changed to 1
            //currentX_Value = currentX_Value + (1.0 * (xAxisMax * 1.0) / (xAxisPixelLength * 1.0) );
            currentX_Value = i * (1.0 * (xAxisMax * 1.0) / (xAxisPixelLength * 1.0) );
        }
        */

}

//Function to graph
double Function(double x)
{

    wxString inputText = frame->MainEditBox->GetValue();

    if(wxGetKeyState((wxKeyCode)'p'))
    {
        equationM = std::string(inputText.mb_str(wxConvUTF8));
    }

    double y = Calculate(equationM, x).getAnswer();

    return y;
}

//Draw Axis to setup graph
void GraphSetup(wxDC& dc)
{

    //Point of graphs origin
    wxPoint origin(2 * (mainFrameWidth/10), 8 * (mainFrameHeight/10));

    //Points to describe where y axis will be

    wxPoint yAxisStartPoint = origin;
    wxPoint yAxisEndPoint(origin.x, mainFrameHeight/10);

    //Points to describe where x axis will be
    wxPoint xAxisStartPoint = origin;
    wxPoint xAxisEndPoint(9 * (mainFrameWidth/10), origin.y);

    //Draw the Axis lines
    //yAxis Line
    dc.DrawLine(yAxisStartPoint, yAxisEndPoint);

    //xAxis Line
    dc.DrawLine(xAxisStartPoint, xAxisEndPoint);

}

void BasicDrawPane::render( wxDC& dc )
{

    Calculate();

    //Constantly update the size of frame
    mainFrameWidth = sizer->GetSize().x;
    mainFrameHeight = sizer->GetSize().y;

    //Move the textBox to keep in frame
    frame->textBoxResize();

    //Setup pen to draw
    wxPen pen;
    pen.SetColour(255,255,255);
    pen.SetWidth(1);
    pen.SetStyle(wxSOLID);
    dc.SetPen(pen);
    
    wxColour backgroundColor(0, 0, 0, 255);
    wxBrush backgroundBrush(black, wxBRUSHSTYLE_SOLID);
    dc.SetBackground( backgroundBrush );
    dc.SetTextForeground( *wxWHITE );
    dc.Clear();

    DrawFunction(dc, xAxisMax, yAxisMax, numX_Intervals, numY_Intervals);
    GraphSetup(dc);

}