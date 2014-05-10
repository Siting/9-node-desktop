clear all
clc

dbstop if error

global full_range

% set parameters
networkID = '9Node-network';
numNodes = 9;
numFacilities = 3;
numRoutes = 5;       % number of candidate routes

numStations = 0;     % number of stations to locate
numPads = numFacilities - numStations;         % number of pads(routes) to locate
full_range = 5;      % set full capacity vehicle range (in mile)
c_station = 2;       % cost for charging station
c_pad = 5;           % cost for charging pad

% load graph
load([networkID, '-graph.mat']);

% load links
% map keys: linkIDs
% map structure: linkID, incomingNode, outgoingNode, lengthInMiles, fuelCost
[LINK] = loadLinks(linkMap);

% load linkID look up matrix
% linkIDMatrix(incomingNode, outgoingNode) = linkID
load('linkIDMatrix.mat');

% load shortest paths == all flow pairs
% matrix, each row: [O, D, cost, traveled nodes]
load('shortest_paths_matrix.mat');

% load unsorted flows
% matrix, each row: [flowID, flow volume]
load('generated_flows_all.mat');

% load sorted flows
% matrix, each row: [flowID, flow volume]
load('generated_flows_sorted.mat');
topFlowIDs = sortedFlows(1:numRoutes, 1);   % retrive candidate route/flow IDs

% retrieve top k flow info: nodes + links
% map keys: flowIDs
% map structure TOP_FLOWS: flowID, origin, destination, cost, nodes, links
% topFlowIDs = [1;2;4];  % for testing
[TOP_FLOWS] = retriveFlows(topFlowIDs, shortest_paths_matrix, linkIDMatrix);

% pre-generate b_qh, a_hp
[b_qh, a_hp] = pregenerateCoefficientMatrix(shortest_paths_matrix, TOP_FLOWS, numNodes,...
    numStations, numPads);

% retrieve all flows info: nodes + links
% map keys: flowIDs
% map structure ALL_FLOWS: flowID, origin, destination, cost, nodes, links
size(shortest_paths_matrix,1);
flowIDs = [1:size(shortest_paths_matrix,1)];
[ALL_FLOWS] = retriveFlows(flowIDs, shortest_paths_matrix, linkIDMatrix);

% filter out ineligible combinatinos
[b_qh, a_hp] = filterCombinations(b_qh, a_hp, ALL_FLOWS, TOP_FLOWS, numNodes, numRoutes, LINK);

% compute refuled flow for each combination
% map keys: combinationID
% map structure: combinationID, refuled total flow, refueled flow ids
% comMatrix, each row: [combinationID, cost, totalRefueledFlow]
[COMBINATION, comMatrix] = generateCombinations(b_qh, a_hp, flows, numNodes, TOP_FLOWS);

% sort combinations
comIDs_sorted = sortrows(comMatrix,2);
comIDs_sorted = flipud(comIDs_sorted);

% save variables
save('./result/b_qh', 'b_qh');
dlmwrite('./result/b_qh.txt', b_qh);
save('./result/a_hp', 'a_hp');
dlmwrite('./result/a_hp.txt', a_hp);
save('./result/COMBINATION', 'COMBINATION');
save('./result/comIDs_sorted', 'comIDs_sorted');
dlmwrite('./result/comIDs_sorted.txt', comIDs_sorted);


