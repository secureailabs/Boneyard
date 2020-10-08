import {Meteor} from "meteor/meteor";

Meteor.methods({
    federated_granular(nodes, master, donorAcc) {
        var Future = Npm.require('fibers/future');
        var enclave1_result1 = new Future();
        var enclave1_result2 = new Future();
        var enclave1_result3 = new Future();

        var enclave2_result1 = new Future();
        var enclave2_result2 = new Future();
        var enclave2_result3 = new Future();
        var final_result = new Future();


        Meteor.call("userRunModelFile",
            nodes[0].pubKey, nodes[0].trainer, nodes[0].model, (error, result) => {
                if (error) {
                    console.log("error:", error);
                    enclave1_result1.return(null)
                } else if (result.error) {
                    console.log("error:", result.error);
                    enclave1_result1.return(null)
                } else {
                    console.log("Run model returned.");
                    Meteor.call("userLoadFullFile", "output_UC.csv", nodes[0].pubKey, donorAcc, (error1, res) => {
                        if (error1) {
                            console.log("error:", error1);
                            enclave1_result1.return(null)
                        } else if (res.error) {
                            console.log("error:", res.error);
                            enclave1_result1.return(null)
                        } else {
                            enclave1_result1.return(res.result)
                            Meteor.call("userLoadFullFile", "output_nonIBD.csv", nodes[0].pubKey, donorAcc, (error2, res2) => {
                                if (error2) {
                                    console.log("error:", error2);
                                    enclave1_result2.return(null)
                                } else if (res2.error) {
                                    console.log("error:", res2.error);
                                    enclave1_result2.return(null)
                                } else {
                                    enclave1_result2.return(res2.result)
                                    Meteor.call("userLoadFullFile", "output_CD.csv", nodes[0].pubKey, donorAcc, (error3, res3) => {
                                        if (error3) {
                                            console.log("error:", error3);
                                            enclave1_result3.return(null)
                                        } else if (res3.error) {
                                            console.log("error:", res3.error);
                                            enclave1_result3.return(null)
                                        } else {
                                            enclave1_result3.return(res3.result)
                                        }
                                    })
                                }
                            })
                        }
                    })
                }
            });

        if (nodes.length > 1) {
            Meteor.call("userRunModelFile",
                nodes[1].pubKey, nodes[1].trainer, nodes[1].model, (error, result) => {
                    if (error) {
                        console.log("error:", error);
                        enclave2_result1.return(null)
                    } else if (result.error) {
                        console.log("error:", result.error);
                        enclave2_result1.return(null)
                    } else {
                        console.log("Run model returned.");
                        Meteor.call("userLoadFullFile", "output_UC.csv", nodes[1].pubKey, donorAcc, (error1, res) => {
                            if (error1) {
                                console.log("error:", error1);
                                enclave2_result1.return(null)
                            } else if (res.error) {
                                console.log("error:", res.error);
                                enclave2_result1.return(null)
                            } else {
                                enclave2_result1.return(res.result)
                                Meteor.call("userLoadFullFile", "output_nonIBD.csv", nodes[1].pubKey, donorAcc, (error2, res2) => {
                                    if (error2) {
                                        console.log("error:", error2);
                                        enclave2_result2.return(null)
                                    } else if (res2.error) {
                                        console.log("error:", res2.error);
                                        enclave2_result2.return(null)
                                    } else {
                                        enclave2_result2.return(res2.result)
                                        Meteor.call("userLoadFullFile", "output_CD.csv", nodes[1].pubKey, donorAcc, (error3, res3) => {
                                            if (error3) {
                                                console.log("error:", error3);
                                                enclave2_result3.return(null)
                                            } else if (res3.error) {
                                                console.log("error:", res3.error);
                                                enclave2_result3.return(null)
                                            } else {
                                                enclave2_result3.return(res3.result)
                                            }
                                        })
                                    }
                                })
                            }
                        })
                    }
                })

        }

        let enclave1_output1 = enclave1_result1.wait();
        let enclave1_output2 = enclave1_result2.wait();
        let enclave1_output3 = enclave1_result3.wait();

        let enclave2_output1 = undefined;
        let enclave2_output2 = undefined;
        let enclave2_output3 = undefined;
        if (nodes.length > 1) {
            enclave2_output1 = enclave2_result1.wait();
            enclave2_output2 = enclave2_result2.wait();
            enclave2_output3 = enclave2_result3.wait();
        }
        // if (nodes.length > 2)
        //     output3 = result3.wait();


        console.log(enclave1_output1);
        console.log(enclave1_output2);
        console.log(enclave1_output3);

        console.log(enclave2_output1);
        console.log(enclave2_output2);
        console.log(enclave2_output3);

        let buffOut1 = new Buffer(enclave1_output1)
        let buffOut2 = new Buffer(enclave1_output2)
        let buffOut3 = new Buffer(enclave1_output3)
        let buffOut4 = new Buffer(enclave2_output1)
        let buffOut5 = new Buffer(enclave2_output2)
        let buffOut6 = new Buffer(enclave2_output3)
        Meteor.call("userShareDataFrontEndE2E",
            master.pubKey, donorAcc, "enclave1_UC.csv", buffOut1.toString('base64'), (error1, res1) => {
                if (error1) {
                    console.log("error:", error1);
                    final_result.return(null)
                } else if (res1.error) {
                    console.log("error:", res1.error);
                    final_result.return(null)
                } else {
                    console.log("First output uploaded to master.");
                    Meteor.call("userShareDataFrontEndE2E",
                        master.pubKey, donorAcc, "enclave1_nonIBD.csv", buffOut2.toString('base64'), (error2, res2) => {
                            if (error2) {
                                console.log("error:", error2);
                                final_result.return(null)
                            } else if (res2.error) {
                                console.log("error:", res2.error);
                                final_result.return(null)
                            } else {
                                console.log("Second output uploaded to master.");
                                Meteor.call("userShareDataFrontEndE2E",
                                    master.pubKey, donorAcc, "enclave1_CD.csv", buffOut3.toString('base64'), (error3, res3) => {
                                        if (error3) {
                                            console.log("error:", error3);
                                            final_result.return(null)
                                        } else if (res3.error) {
                                            console.log("error:", res3.error);
                                            final_result.return(null)
                                        } else {
                                            console.log("Third output uploaded to master.");
                                            Meteor.call("userShareDataFrontEndE2E",
                                                master.pubKey, donorAcc, "enclave2_UC.csv", buffOut4.toString('base64'), (error4, res4) => {
                                                    if (error4) {
                                                        console.log("error:", error4);
                                                        final_result.return(null)
                                                    } else if (res4.error) {
                                                        console.log("error:", res4.error);
                                                        final_result.return(null)
                                                    } else {
                                                        console.log("Fourth output uploaded to master.");
                                                        Meteor.call("userShareDataFrontEndE2E",
                                                            master.pubKey, donorAcc, "enclave2_nonIBD.csv", buffOut5.toString('base64'), (error5, res5) => {
                                                                if (error5) {
                                                                    console.log("error:", error5);
                                                                    final_result.return(null)
                                                                } else if (res5.error) {
                                                                    console.log("error:", res5.error);
                                                                    final_result.return(null)
                                                                } else {
                                                                    console.log("Fifth output uploaded to master.");
                                                                    Meteor.call("userShareDataFrontEndE2E",
                                                                        master.pubKey, donorAcc, "enclave2_CD.csv", buffOut6.toString('base64'), (error6, res6) => {
                                                                            if (error6) {
                                                                                console.log("error:", error6);
                                                                                final_result.return(null)
                                                                            } else if (res6.error) {
                                                                                console.log("error:", res6.error);
                                                                                final_result.return(null)
                                                                            } else {
                                                                                console.log("Sixth output uploaded to master.");
                                                                                Meteor.call("userRunModelFile",
                                                                                    master.pubKey, master.trainer, "model.py", (error7, res7) => {
                                                                                        if (error7) {
                                                                                            console.log("error:", error7);
                                                                                            final_result.return(null)
                                                                                        } else if (res7.error) {
                                                                                            console.log("error:", res7.error);
                                                                                            final_result.return(null)
                                                                                        } else {
                                                                                            console.log("Master run successful");
                                                                                            console.log(res7.result);
                                                                                            final_result.return(res7.result)
                                                                                        }
                                                                                    })
                                                                            }
                                                                        })
                                                                }

                                                            })
                                                    }
                                                })

                                        }
                                    })
                            }
                        })


                }
            })

        return final_result.wait()
    }
})

Meteor.methods({
    downloadFinalResult(master,donorAcc) {
        var Future = Npm.require('fibers/future');
        var result = new Future();

        Meteor.call("userLoadFullFile", "hubOutput.csv", master.pubKey, donorAcc, (err, res) => {
            if (err) {
                console.log("error:", err);
                result.return(err)
            } else if (res.error) {
                console.log("error:", res.error);
                result.return (res.err)
            } else {
                console.log(res)
                result.return(res)
            }
        })
        return result.wait()
    }

})