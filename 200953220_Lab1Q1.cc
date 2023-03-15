#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

NS_LOG_COMPONENT_DEFINE ("Lab1 Question1");

using namespace ns3;
int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  Ptr<Node> S = CreateObject<Node> ();
  Ptr<Node> r = CreateObject<Node> ();
  Ptr<Node> D = CreateObject<Node> ();

  NodeContainer net1 (S, r);
  NodeContainer net2 (r, D);
  NodeContainer all (S, r, D);
  
  PointToPointHelper l1;
  l1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  l1.SetChannelAttribute("Delay",StringValue("50ms"));
  NetDeviceContainer devices1;
  devices1 = l1.Install(net1);
  
  PointToPointHelper l2;
  NetDeviceContainer devices2;
  l1.SetDeviceAttribute("DataRate",StringValue("100Kbps"));
  l1.SetChannelAttribute("Delay",StringValue("5ms"));
  devices2 = l2.Install(net2);
  
  InternetStackHelper inet_stack;
  inet_stack.Install(all);
  
   Ipv4AddressHelper address1;
  address1.SetBase ("192.168.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);

  
  Ipv4AddressHelper address2;
  address2.SetBase ("192.169.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (D);
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));
  
  UdpEchoClientHelper echoClient (interfaces2.GetAddress(1),9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  
  ApplicationContainer clientApps = echoClient.Install (S);
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (10.0));
  
  Simulator::Run();
 }
