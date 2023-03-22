#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/application-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("Lab1Q1");

int main(int argc, char* argv[])
{
  CommandLine cmd;
  cmd.Parse(argc,argv);
  Time::SetResolution(Time::NS);
  NodeContainer nodes;
  nodes.Create(5);
  
  PointToPointHelper p2p1;
  p2p1.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
  p2p1.SetChannelAttribute("Delay", StringValue("50ms"));
  
  PointToPointHelper p2p2;
  p2p1.SetDeviceAttribute("DataRate", StringValue("100Kbps"));
  p2p1.SetChannelAttribute("Delay", StringValue("5ms"));
  
  NetDeviceContainer d1 = p2p1.Install(nodes.Get(0),nodes.Get(1));
  NetDeviceContainer d2 = p2p2.Install(nodes.Get(1),nodes.Get(2));
  
  InternetStackHelper stack;
  stack.Install(nodes);
  
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  
  Ipv4InterfaceContainer IF1 = address.Assign(d1);
  Ipv4InterfaceContainer IF2 = address.Assign(d2);
  
  UdpEchoServer echoServer(9);
  
  ApplicationContainer serverApps = echoServer.Install(nodes.Get(2));
  serverApps.Start(Seconds(1.0));
  serverApps.Stop(Seconds(10.0));
  
  UdpEchoClienHelper echoClient(IF2.GetAddress(d1),9);
  echoClient.SetAttribute("Max Packets",UintegerValue(6));
  echoClient.SetAttribute("Interval", UintergerValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize", UintergerValue(1024));
  
  ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(2.0));
  
  Simulator::Run();
  Simulator::Destroy();
  return 0;
 }
  
  
