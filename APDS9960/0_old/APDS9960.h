#include <stdint.h>

/* Direction definitions */
enum {
  DIR_NONE,
  DIR_LEFT,
  DIR_RIGHT,
  DIR_UP,
  DIR_DOWN,
  DIR_NEAR,
  DIR_FAR,
  DIR_ALL
};

/* State definitions */
enum {
  NA_STATE,
  NEAR_STATE,
  FAR_STATE,
  ALL_STATE
};

/* Container for gesture data */
typedef struct gesture_data_type {
    uint8_t u_data[32];
    uint8_t d_data[32];
    uint8_t l_data[32];
    uint8_t r_data[32];
    uint8_t index;
    uint8_t total_gestures;
    uint8_t in_threshold;
    uint8_t out_threshold;
} gesture_data_type;


class apds9960:public i2cComm{

public:

    /* Initialization methods */
    apds9960(int devId);
    ~apds9960();
    bool init();
    uint8_t getMode();
    bool setMode(uint8_t mode, uint8_t enable);
    
    /* Turn the APDS-9960 on and off */
    bool enablePower();
    bool disablePower();
    
    /* Enable or disable specific sensors */
    bool enableLightSensor(bool interrupts = false);
    bool disableLightSensor();
    bool enableProximitySensor(bool interrupts = false);
    bool disableProximitySensor();
    bool enableGestureSensor(bool interrupts = true);
    bool disableGestureSensor();
    
    /* LED drive strength control */
    uint8_t getLEDDrive();
    bool setLEDDrive(uint8_t drive);
    uint8_t getGestureLEDDrive();
    bool setGestureLEDDrive(uint8_t drive);
    
    /* Gain control */
    uint8_t getAmbientLightGain();
    bool setAmbientLightGain(uint8_t gain);
    uint8_t getProximityGain();
    bool setProximityGain(uint8_t gain);
    uint8_t getGestureGain();
    bool setGestureGain(uint8_t gain);
    
    /* Get and set light interrupt thresholds */
    bool getLightIntLowThreshold(uint16_t &threshold);
    bool setLightIntLowThreshold(uint16_t threshold);
    bool getLightIntHighThreshold(uint16_t &threshold);
    bool setLightIntHighThreshold(uint16_t threshold);
    
    /* Get and set proximity interrupt thresholds */
    bool getProximityIntLowThreshold(uint8_t &threshold);
    bool setProximityIntLowThreshold(uint8_t threshold);
    bool getProximityIntHighThreshold(uint8_t &threshold);
    bool setProximityIntHighThreshold(uint8_t threshold);
    
    /* Get and set interrupt enables */
    uint8_t getAmbientLightIntEnable();
    bool setAmbientLightIntEnable(uint8_t enable);
    uint8_t getProximityIntEnable();
    bool setProximityIntEnable(uint8_t enable);
    uint8_t getGestureIntEnable();
    bool setGestureIntEnable(uint8_t enable);
    
    /* Clear interrupts */
    bool clearAmbientLightInt();
    bool clearProximityInt();
    
    /* Ambient light methods */
    bool readAmbientLight(uint16_t &val);
    bool readRedLight(uint16_t &val);
    bool readGreenLight(uint16_t &val);
    bool readBlueLight(uint16_t &val);
    
    /* Proximity methods */
    bool readProximity(uint8_t &val);
    
    /* Gesture methods */
    bool isGestureAvailable();
    int readGesture();
    
private:

    /* Gesture processing */
    void resetGestureParameters();
    bool processGestureData();
    bool decodeGesture();

    /* Proximity Interrupt Threshold */
    uint8_t getProxIntLowThresh();
    bool setProxIntLowThresh(uint8_t threshold);
    uint8_t getProxIntHighThresh();
    bool setProxIntHighThresh(uint8_t threshold);
    
    /* LED Boost Control */
    uint8_t getLEDBoost();
    bool setLEDBoost(uint8_t boost);
    
    /* Proximity photodiode select */
    uint8_t getProxGainCompEnable();
    bool setProxGainCompEnable(uint8_t enable);
    uint8_t getProxPhotoMask();
    bool setProxPhotoMask(uint8_t mask);
    
    /* Gesture threshold control */
    uint8_t getGestureEnterThresh();
    bool setGestureEnterThresh(uint8_t threshold);
    uint8_t getGestureExitThresh();
    bool setGestureExitThresh(uint8_t threshold);
    
    /* Gesture LED, gain, and time control */
    uint8_t getGestureWaitTime();
    bool setGestureWaitTime(uint8_t time);
    
    /* Gesture mode */
    uint8_t getGestureMode();
    bool setGestureMode(uint8_t mode);

	    /* Members */
    gesture_data_type gesture_data_;
    int gesture_ud_delta_;
    int gesture_lr_delta_;
    int gesture_ud_count_;
    int gesture_lr_count_;
    int gesture_near_count_;
    int gesture_far_count_;
    int gesture_state_;
    int gesture_motion_;
	
};
