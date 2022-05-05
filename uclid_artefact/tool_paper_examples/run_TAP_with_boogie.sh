echo "This script runs the trusted abstract platform proofs using boogie"
echo "it needs to download and install boogie"
echo "this script is included only for comparison, and is not part of the UCLID5 reproduction"
read -r -p "Do you wish to continue? [y/N] " response
case "$response" in
    [yY][eE][sS]|[yY]) 
        echo "ok, continuing"
        ;;
    *)
        exit 1
        ;;
esac

echo "installing boogie"
sudo apt update
sudo apt install boogie


echo "Verifying TAP models in Boogie ..."
cd TAP/AbstractPlatform
time boogie CPU.bpl CPUImpl.bpl Types.bpl ../Common/Cache.bpl ../Common/CacheImpl.bpl ../Common/Common.bpl ../Common/Types.bpl

echo "Verifying integrity proof model in Boogie ..."
time boogie IntegrityProof.bpl CPU.bpl CPUImpl.bpl Types.bpl ../Common/Cache.bpl ../Common/CacheImpl.bpl ../Common/Common.bpl ../Common/Types.bpl ProofCommon.bpl /proc:ProveIntegrity
