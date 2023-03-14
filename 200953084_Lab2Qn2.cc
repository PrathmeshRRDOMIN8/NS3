#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"


NS_LOG_COMPONENT_DEFINE ("Lab2 Question1");

using namespace ns3;
int main(int argc, char** argv)
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
	Time::SetResolution (Time::NS);
	LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  	std::string animFile ="lab2qn2_200953084. xml";

	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
	PointToPointStarHelper star (12, pointToPoint);
	
	InternetStackHelper inet;
	star.InstallStack(inet);
	
	star.AssignIpv4Addresses(Ipv4AddressHelper("192.168.1.0","255.255.255.0"));	
	
	Address hubLocalAddress(InetSocketAddress(Ipv4Address::GetAny(),1337));
	PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", hubLocalAddress);
	
	UdpEchoServerHelper echoServer (9);
	ApplicationContainer serverApps = echoServer.Install (star.GetHub());
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (30.0));
	  
	for (int i = 0; i < 12; i++)
	{
		 UdpEchoClientHelper echoClient (star.GetHubIpv4Address(i),9);
		  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
		  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
		  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
		  ApplicationContainer clientApps = echoClient.Install (star.GetSpokeNode(i));
		  clientApps.Start (Seconds (1.0));
		  clientApps.Stop (Seconds (30.0));
	}
  	AnimationInterface anim (animFile ); 
/*

  	anim.SetConstantPosition (all.Get(0), 0,100);
    	anim.SetConstantPosition (all.Get(6), 100*6,100);
  	for (int i = 1; i < 6; i++)
	{
		anim.SetConstantPosition (all.Get(i), 100*i,0);
	}
*/
	Simulator::Run();
}

