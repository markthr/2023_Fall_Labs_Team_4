#ifndef LINE_DET_H
#define LINE_DET_H

class Line_Detector {
    public:
        /**
         * @brief Construct a new Line_Detector object
         * 
         * @param det_size The number of detectors that make up the line following sensor.
         * @param buf_len  The number of past values to store, must be at least 1.
         */
        Line_Detector(int det_size, int buf_len);

        ~Line_Detector();
        
        /**
         * @brief 
         * 
         * @param det_vals Must be length det_size, the new values read in from the ADC
         * @return float A value in [-1, 1]
         */
        float detection(const int det_vals[]);
        
        /**
         * @brief 
         * 
         * @param det 
         * @param det_num 
         * @param det_vals 
         */
        void multi_detection(float det[], int det_num, const int det_vals[]);

        /**
         * @brief Performs multidetections on whatever values are in the buffer
         * 
         * @param det 
         * @param det_num 
         */
        void multi_detection(float det[], int det_num);

        /**
         * @brief Performs single detection on whatever values are in the buffer
         * 
         * @return float A value in [-1, 1]
         */
        float detection();
    protected:
        virtual float detect() = 0;
        virtual void multi_detect(float det[]){};

    private:
        void accept(const int det_vals[]);
        int det_size;
        int n_buf;
        int i_buf;
        bool buffer_init;
        int* buffer; // pointer to 2D array
    
};

#endif