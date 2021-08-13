#include <Wire.h>
#include <Adafruit_LPS2X.h>

LPS ps;

const float r_specific = 287.058;
const float gas_molar_mass = 0.004002602;
const float universal_gas_constant = 8.3145;
const float gravitational_constant = 6.67408e-11;
const float earth_mass = 5.972e24;
const float earth_radius = 6371e3;
const float balloon_volume = 3.20256;
const float load_mass = 0.4;
const float sphere_drag_coefficient = 0.47;
const float balloon_area = 2.62677;

float temperature = 0;
float air_pressure = 0;
float altitude = 0;

bool ascending = true;

float pos_x;
float pos_y;

float vel_x;
float vel_y;

float acc_x;
float acc_y ;

void setup() {
  Serial.begin(9600);
  Wire.begin();=

  if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }

  ps.enableDefault();
}

void loop() {
  delay(1000);
  
  float air_pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();
  
  float air_density = air_pressure / (r_specific * temperature);
  float gas_density = air_pressure * gas_molar_mass / (universal_gas_constant * temperature);
  float gravitational_acceleration = gravitational_constant * earth_mass / (earth_radius + altitude) * (earth_radius + altitude);
  float gravity = (air_pressure * balloon_volume + load_mass) * gravitational_acceleration;
  float net_force = 0;
  
  if(ascending)
  {
    float buoyancy_force = air_pressure * balloon_volume * gravitational_acceleration;
    float drag_force = 0.5 * air_pressure * vel_y * sphere_drag_coefficient * balloon_area;
    net_force = buoyancy_force - gravity - drag_force;
  }
  else
  {
    float buoyancy_force = air_pressure * balloon_volume * gravitational_acceleration;
    float drag_force = 0.5 * air_pressure * vel_y * sphere_drag_coefficient * balloon_area;
    net_force = drag_force - gravity;
  }
  acc_y += net_force / (air_pressure * balloon_volume + load_mass);
  vel_y += acc_y;
  pos_y += vel_y;
}
