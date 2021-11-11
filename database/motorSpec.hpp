#pragma once

namespace ishihalib {

namespace motor_spec {

struct motorSpec_t {
	const float R;	 //抵抗
	const float Kv;	 //(V)/(rps)
	const float L;	 //インダクタ

	const float Kt;	 //Nm/A
};

struct RZ735VA : public motorSpec_t {
	static const float R = 18. / 156;						 //抵抗
	static const float Kv = (18. - R * 2.8) / (20400 / 60);	 //(V)/(rps)
	static const float L;									 //インダクタ

	static const float Kt = 1.265 / 156.;  //Nm/A
};

};	// namespace motor_spec

};	// namespace ishihalib