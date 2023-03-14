#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
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
  	std::string animFile ="lab2qn1_200953084. xml";
	NodeContainer all,csmaNodes, p2p_1,p2p_2;
	all.Create(7);
	
	

	for (int i = 1; i < 6;i++)
	{
		csmaNodes.Add(all.Get(i));
	}
	CsmaHelper csma;
	PointToPointHelper p2phelper_1, p2phelper_2;
	p2p_1.Add(all.Get(0));
	p2p_1.Add(all.Get(1));
	
	p2p_2.Add(all.Get(1));
	p2p_2.Add(all.Get(2));
	

	  p2phelper_1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
	  p2phelper_1.SetChannelAttribute("Delay",StringValue("2ms"));
	NetDeviceContainer csmaDevices = csma.Install(csmaNodes);
	NetDeviceContainer p2p1_devices = p2phelper_1.Install(p2p_1);

	NetDeviceContainer p2p2_devices = p2phelper_2.Install(p2p_2);
	
	InternetStackHelper inet_stack;
	inet_stack.Install(all);
	

	Ipv4AddressHelper address;
	address.SetBase("192.168.1.0","255.255.255.0");
	
	Ipv4InterfaceContainer csmaInterfaces = address.Assign(csmaDevices);
	
	Ipv4AddressHelper address2;
	address2.SetBase("10.0.0.0","255.0.0.0");
	
	Ipv4InterfaceContainer p2p1if = address2.Assign(p2p1_devices);
	
	Ipv4AddressHelper address3;
	address3.SetBase("20.0.0.0","255.0.0.0");
	Ipv4InterfaceContainer p2p2if = address3.Assign(p2p2_devices);
	Ipv4GlobalRoutingHelper::PopulateRoutingTables();
	

	UdpEchoServerHelper echoServer (9);

	ApplicationContainer serverApps = echoServer.Install (all.Get(0));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (30.0));
	  

	 
	  for (int i = 0; i < 7; i++)
	  {

	  	  UdpEchoClientHelper echoClient (p2p1if.GetAddress(0),9);
		  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
		  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
		  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
		  ApplicationContainer clientApps = echoClient.Install (all.Get(i));
		  clientApps.Start (Seconds (1.0));
		  clientApps.Stop (Seconds (30.0));
		  

  	}
  	AnimationInterface anim (animFile ); 
  	anim.SetConstantPosition (all.Get(0), 0,100);
    	anim.SetConstantPosition (all.Get(6), 100*6,100);
  	for (int i = 1; i < 6; i++)
	{
		anim.SetConstantPosition (all.Get(i), 100*i,0);
	}
	
  	Simulator::Run();
}
