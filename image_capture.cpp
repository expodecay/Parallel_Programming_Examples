#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>
// highgui - an interface to video and image capturing.

#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/videoio/videoio_c.h>
// The header files for performing input and output.

int main()
{
    cv::Mat tmp;
    cv::VideoCapture cap("C:/Users/rickr/Documents/MPI/mpi_windows_x64/video/test_flight_003.MOV");
    // cap is the object of class video capture that tries to capture Bumpy.mp4
    if (!cap.isOpened())  // isOpened() returns true if capturing has been initialized.
    {
        std::cout << "Cannot open the video file. \n";
        return -1;
    }

    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
    // The function get is used to derive a property from the element.
    // Example:
    // CV_CAP_PROP_POS_MSEC :  Current Video capture timestamp.
    // CV_CAP_PROP_POS_FRAMES : Index of the next frame.

    //cv::namedWindow("A_good_name", cv::WINDOW_AUTOSIZE); //create a window called "MyVideo"

    // first argument: name of the window.
    // second argument: flag- types: 
    // WINDOW_NORMAL : The user can resize the window.
    // WINDOW_AUTOSIZE : The window size is automatically adjusted to fitvthe displayed image() ), and you cannot change the window size manually.
    // WINDOW_OPENGL : The window will be created with OpenGL support.

    int i = 0;
    //while (true)
    for(i = 0; i < 127; i++)
    {
        if (!(i % 1)) {
            cv::Mat frame, resized;
            // Mat object is a basic image container. frame is an object of Mat.



            if (!cap.read(frame)) // if not success, break loop
            // read() decodes and captures the next frame.
            {
                std::cout << "\n Cannot read the video file. \n";
                break;
            }



            cv::String image_path = "frames/" + std::to_string(i) + ".jpg";


            if (!frame.empty()) {
                tmp = frame;

                // imshow("Image", tmp);
                 //waitKey(0);
                resize(frame, resized, cv::Size(1920, 1080));
                //cv::imshow("resized", resized);

                //std::cout << "Frame # " << std::to_string(i) << " SUCCESS " << std::endl;


                std::cout << "Frame # " << std::to_string(i) << " SUCCESS " << std::endl;
                std::cout << i % 5 + "\n";
                cv::String path = "C:/Users/rickr/Documents/MPI/mpi_windows_x64/x64/Release/sample_images/";
                imwrite(path + cv::format("img_%d.png", i), tmp);


                tmp.release();
            }

        }

        if (cv::waitKey(30) == 27) // Wait for 'esc' key press to exit
        {
            break;
        }
       // i++;
    }

    return 0;
}
// END OF PROGRAM